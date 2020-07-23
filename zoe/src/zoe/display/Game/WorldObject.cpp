//
// Created by Florian on 11.07.2020.
//

#include "WorldObject.h"

namespace Zoe{

WorldObject::WorldObject() = default;

void WorldObject::onDraw(const Camera &camera) {

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
