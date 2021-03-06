//
// Created by Florian on 11.07.2020.
//

#include "WorldObject.h"
#include "../../render/Material.h"
#include "../../core/Application.h"
#include "ModelComponent.h"

#include "../../../platform/OpenGL/OpenGLVertexArrayImpl.h"

namespace Zoe {

WorldObject::WorldObject() {
    init = false;
}

void WorldObject::onDraw(const Camera& camera) {
    if (init) {
        mat4x4 modelMatrix = model.getModelMatrix();
        vec3 pos = getWorldPosition();
        model.setModelMatrix(modelMatrix * Zoe::translate3D(pos.x, pos.y, pos.z));
        camera.draw(model);
        model.setModelMatrix(modelMatrix);
    }
}

void WorldObject::onUpdate(double time) {
}

void WorldObject::onInputEvent(Event& event) {

}

void WorldObject::fill(const XMLNode& node) {

}

void WorldObject::postFill() {
    for (const auto& child: getChildren()) {
        //could be optimised with member var and reinterpret_cast
        //is only called when object is loaded from xml file
        if (const auto& modelComponent = std::dynamic_pointer_cast<ModelComponent>(child); modelComponent) {
            model = modelComponent->getModel();
            init = true;
            break;
        }
    }
}
}
