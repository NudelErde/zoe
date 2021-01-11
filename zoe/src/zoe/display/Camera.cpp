//
// Created by Florian on 11.07.2020.
//

#include "Camera.h"
#include "../render/Model.h"
#include <cmath>

namespace Zoe {

const mat4x4& Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::setProjectionMatrix(const mat4x4& projection) {
    projectionMatrix = projection;
}

void Camera::setViewMatrix(const mat4x4& view) {
    viewMatrix = view;
    invViewMatrix = view.inverse();
}

const mat4x4& Camera::getViewMatrix() const {
    return viewMatrix;
}

const mat4x4& Camera::getInvViewMatrix() const {
    return invViewMatrix;
}

void Camera::draw(Model& model) const {
    if (render) {
        model.getMaterial().bind(*this, model.getModelMatrix());
        getRender()->draw(*model.getVertexArray(), *model.getMaterial().getShader());
    }
}
void Camera::fill(const XMLNode& node) {}

void Camera::postFill() {}

void Camera::onUpdate(double time) {
    updateCameraMatrix();
}

void Camera::onDraw(const Camera& camera) {}

void Camera::onInputEvent(Event& event) {}

Camera::~Camera() = default;

Camera::Camera() = default;

//Camera2D

Camera2D::Camera2D(const vec2& position, const float& width, const float& height) : width(width), height(height) {
    setProjectionMatrix(scale3D(2 / width, 2 / height, 1));
    setPosition(vec3(position));
    Camera2D::updateCameraMatrix();
}

void Camera2D::updateCameraMatrix() {
    vec3 pos = getWorldPosition();
    setViewMatrix(translate3D(-pos.x, -pos.y, 0));
}
void Camera2D::fill(const XMLNode& node) {
    if (node.attributes.count("x") > 0) {
        position.x = std::stof(node.attributes.at("x"));
    }
    if (node.attributes.count("y") > 0) {
        position.y = std::stof(node.attributes.at("y"));
    }

    if (node.attributes.count("width") > 0) {
        width = std::stof(node.attributes.at("width"));
    }
    if (node.attributes.count("height") > 0) {
        height = std::stof(node.attributes.at("height"));
    }
    updateCameraMatrix();
    setProjectionMatrix(scale3D(2 / width, 2 / height, 1));
}

//Camera3D

Camera3D::Camera3D(const vec3& pos, const vec3& rot, const float& fov, const float& ratio) : rotation(rot), fov(fov),
                                                                                             ratio(ratio) {
    setProjectionMatrix(scale3D(1,1,0.01f) * perspective(1, 2, fov, ratio));
    setPosition(pos);
    Camera3D::updateCameraMatrix();
}

void Camera3D::updateCameraMatrix() {
    vec3 pos = getWorldPosition();
    setViewMatrix(calcViewMatrix(pos, -1 * rotation));
}

void Camera3D::setRotation(const vec3& rot) {
    rotation = rot;
    updateCameraMatrix();
}

void Camera3D::setPitch(const float& pitch) {
    rotation.x = pitch;
    if (rotation.x > 2 * PI)
        rotation.x -= 2 * PI;
    updateCameraMatrix();
}

void Camera3D::setYaw(const float& yaw) {
    rotation.y = yaw;
    if (rotation.y > 2 * PI)
        rotation.y -= 2 * PI;
    updateCameraMatrix();
}

void Camera3D::setRoll(const float& roll) {
    rotation.z = roll;
    if (rotation.z > 2 * PI)
        rotation.z -= 2 * PI;
    updateCameraMatrix();
}

void Camera3D::setRotation(const float& pitch, const float& yaw, const float& roll) {
    rotation.x = pitch;
    rotation.y = yaw;
    rotation.z = roll;
    updateCameraMatrix();
}

void Camera3D::setDirection(const vec3& direction) {
    setDirection(direction, 0);
}

void Camera3D::setDirection(const vec3& direction, const float& roll) {
    vec3 d = direction.normalize();
    setRotation(asin(-d.y), atan2(d.x, d.z), roll);
    updateCameraMatrix();
}


const vec3& Camera3D::getRotation() const {
    return rotation;
}

vec3 Camera3D::getDirection() const {
    return (rotateXZ3D(-rotation.y) * rotateYZ3D(-rotation.x) * rotateXY3D(-rotation.z) * vec4({0, 0, 1, 0})).xyz();
}

void Camera3D::setFoV(const float& f) {
    fov = f;
    setProjectionMatrix(perspective(0.01f, 100, fov, ratio));
}

void Camera3D::setRatio(const float& rat) {
    ratio = rat;
    setProjectionMatrix(perspective(0.01f, 100, fov, ratio));
}

void Camera3D::rotate(const vec3& vec) {
    setRotation(getRotation() + vec);
}
void Camera3D::fill(const XMLNode& node) {
    if (node.attributes.count("x") > 0) {
        position.x = std::stof(node.attributes.at("x"));
    }
    if (node.attributes.count("y") > 0) {
        position.y = std::stof(node.attributes.at("y"));
    }
    if (node.attributes.count("z") > 0) {
        position.z = std::stof(node.attributes.at("z"));
    }


    if (node.attributes.count("pitch") > 0) {
        rotation.x = std::stof(node.attributes.at("pitch"));
    }
    if (node.attributes.count("yaw") > 0) {
        rotation.y = std::stof(node.attributes.at("yaw"));
    }
    if (node.attributes.count("roll") > 0) {
        rotation.z = std::stof(node.attributes.at("roll"));
    }

    if (node.attributes.count("fov") > 0) {
        fov = std::stof(node.attributes.at("fov"));
    }
    if (node.attributes.count("ratio") > 0) {
        ratio = std::stof(node.attributes.at("ratio"));
    }
    updateCameraMatrix();
    setProjectionMatrix(scale3D(1,1,0.01f) * perspective(1, 2, fov, ratio));
}

}