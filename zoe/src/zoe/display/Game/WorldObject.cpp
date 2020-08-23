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

void WorldObject::onDraw(const Camera &camera) {
    if (init) {
        camera.draw(model);
    }
}

void WorldObject::onUpdate(double time) {
    static std::chrono::time_point<std::chrono::steady_clock> timePoint = std::chrono::steady_clock::now();
    model.setModelMatrix(rotateXZ3D(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - timePoint).count() / 1000.0f));
    //TODO:: remove this code
}

void WorldObject::onInputEvent(Event &event) {

}

void WorldObject::fill(const XMLNode &node) {

}

void WorldObject::postFill() {
    for (const auto &child: getChildren()) {
        //could be optimised with member var and reinterpret_cast
        //is only called when object is loaded from xml file
        if (const auto &modelComponent = std::dynamic_pointer_cast<ModelComponent>(child); modelComponent) {
            model = modelComponent->getModel();
            init = true;
            break;
        }
    }
}
}
