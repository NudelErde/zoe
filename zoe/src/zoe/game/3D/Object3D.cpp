//
// Created by florian on 13.03.20.
//

#include "Object3D.h"

#include <utility>

namespace Zoe{


    Object3D::~Object3D() = default;

    void Object3D::tick() {}

    Object3D::Object3D(Material material): material(std::move(material)) {}

    Object3D::Object3D(): material() {}
}