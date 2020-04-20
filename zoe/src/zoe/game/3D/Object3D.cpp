//
// Created by florian on 13.03.20.
//

#include "Object3D.h"

#include <utility>

namespace Zoe{


    Object3D::~Object3D() = default;

    void Object3D::tick() {}

    Object3D::Object3D(): material(),model() {}

    Object3D::Object3D(Material material, Model model): material(std::move(material)),model(std::move(model)) {

    }

    void Object3D::draw(Camera &camera) {
        camera.draw(material, model);
    }

}