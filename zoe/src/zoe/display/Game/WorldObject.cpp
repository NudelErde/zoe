//
// Created by Florian on 11.07.2020.
//

#include "WorldObject.h"
#include "../../render/Material.h"
#include "../../core/Application.h"

namespace Zoe{

WorldObject::WorldObject() {
    init = false;
}

void WorldObject::onDraw(const Camera &camera) {
    if(init){
        model.getMaterial().bind(model.getModelMatrix(), camera.getViewMatrix(), camera.getProjectionMatrix());
        camera.getRender()->draw(*model.getVertexArray(), *model.getMaterial().getShader());
    }
}

void WorldObject::onUpdate(double time) {

}

void WorldObject::onInputEvent(Event &event) {

}

void WorldObject::fill(const XMLNode &node) {

}

void WorldObject::postFill() {
    //TODO: search in children for material
}
}
