//
// Created by Florian on 11.07.2020.
//

#include "WorldObject.h"
#include "../../render/Material.h"
#include "../../core/Application.h"

namespace Zoe{

static struct{
    Material defaultMaterial;
    bool init = false;
} data;


WorldObject::WorldObject(){
    if(!data.init){
        data.init = true;
        data.defaultMaterial = Material(Application::getContext().getShader(File("zoe/internal/materials/DefaultMaterial.glsl")));
    }
}

void WorldObject::onDraw(const Camera &camera) {
    //TODO: check if object could use other material
    data.defaultMaterial.setModelMatrix(model.getModelMatrix());
    data.defaultMaterial.setViewMatrix(camera.getViewMatrix());
    data.defaultMaterial.setProjectionMatrix(camera.getProjectionMatrix());
    data.defaultMaterial.bind();
    camera.getRender()->draw(*model.getVertexArray(), *data.defaultMaterial.getShader());
}

void WorldObject::onUpdate(double time) {

}

void WorldObject::onInputEvent(Event &event) {

}

void WorldObject::fill(const XMLNode &node) {

}

void WorldObject::postFill() {
    //TODO: search in children for texture, normalTexture, material
}
}
