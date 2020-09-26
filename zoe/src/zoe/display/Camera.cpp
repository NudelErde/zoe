//
// Created by Florian on 11.07.2020.
//

#include "Camera.h"
#include "../render/Model.h"
#include "../render/Material.h"
#include <cmath>

namespace Zoe {

const mat4x4 &Camera::getProjectionMatrix() const {
    return projectionMatrix;
}

void Camera::setProjectionMatrix(const mat4x4 &projection) {
    projectionMatrix = projection;
}

void Camera::setViewMatrix(const mat4x4 &view) {
    viewMatrix = view;
    invViewMatrix = view.inverse();
}

const mat4x4 &Camera::getViewMatrix() const {
    return viewMatrix;
}

const mat4x4& Camera::getInvViewMatrix() const {
    return invViewMatrix;
}

void Camera::draw(Model &model) const {
    if(render){
        model.getMaterial().bind(*this, model.getModelMatrix());
        getRender()->draw(*model.getVertexArray(), *model.getMaterial().getShader());
    }
}

vec3 Camera::getPosition() const {
    return vec3({invViewMatrix[0][3], invViewMatrix[1][3], invViewMatrix[2][3]});
}

Camera::~Camera() = default;

Camera::Camera() = default;

//Camera2D

Camera2D::Camera2D(const vec2 &position, const float &width, const float &height) : position(position), width(width), height(height) {
    setProjectionMatrix(scale3D(2/width, 2/height, 1));
    setViewMatrix(translate3D(-position.x, -position.y, 0));
}

void Camera2D::setPosition(const vec2 &pos) {
    position = pos;
    setViewMatrix(translate3D(-pos.x, -pos.y, 0));
}

vec3 Camera2D::getPosition() const {
    vec3 res{};
    res.xy = position;
    return res;
}

//Camera3D

Camera3D::Camera3D(const vec3 &pos, const vec3 &rot, const float &fov, const float &ratio) : position(pos),
                                                                                             rotation(rot), fov(fov),
                                                                                             ratio(ratio) {
    setProjectionMatrix(perspective(0.1f, 100, fov, ratio));
    setViewMatrix(calcViewMatrix(pos, -1 * rot));
}

void Camera3D::setPosition(const vec3 &pos) {
    position = pos;
    setViewMatrix(calcViewMatrix(pos, -1 * rotation));
}

void Camera3D::setRotation(const vec3 &rot) {
    rotation = rot;
    setViewMatrix(calcViewMatrix(position, -1 * rot));
}

void Camera3D::setPitch(const float &pitch) {
    rotation.x = pitch;
    if (rotation.x > 2 * PI)
        rotation.x -= 2 * PI;
    setViewMatrix(calcViewMatrix(position, -1 * rotation));
}

void Camera3D::setYaw(const float &yaw) {
    rotation.y = yaw;
    if (rotation.y > 2 * PI)
        rotation.y -= 2 * PI;
    setViewMatrix(calcViewMatrix(position, -1 * rotation));
}

void Camera3D::setRoll(const float &roll) {
    rotation.z = roll;
    if (rotation.z > 2 * PI)
        rotation.z -= 2 * PI;
    setViewMatrix(calcViewMatrix(position, -1 * rotation));
}

void Camera3D::setRotation(const float &pitch, const float &yaw, const float &roll) {
    rotation.x = pitch;
    rotation.y = yaw;
    rotation.z = roll;
    setViewMatrix(calcViewMatrix(position, -1 * rotation));
}

void Camera3D::setDirection(const vec3 &direction) {
    setDirection(direction, 0);
}

void Camera3D::setDirection(const vec3 &direction, const float &roll) {
    vec3 d = direction.normalize();
    setRotation(asin(-d.y), atan2(d.x, d.z), roll);
    setViewMatrix(calcViewMatrix(position, -1 * rotation));
}

vec3 Camera3D::getPosition() const {
    return position;
}

const vec3 &Camera3D::getRotation() const {
    return rotation;
}

vec3 Camera3D::getDirection() const {
    return (rotateXZ3D(rotation.y) * rotateYZ3D(rotation.x) * rotateXY3D(rotation.z) * vec4({0, 0, -1, 0})).xyz;
}

void Camera3D::setFoV(const float &f) {
    fov = f;
    setProjectionMatrix(perspective(0.01f, 100, fov, ratio));
}

void Camera3D::setRatio(const float &rat) {
    ratio = rat;
    setProjectionMatrix(perspective(0.01f, 100, fov, ratio));
}

void Camera3D::move(const vec3 & vec) {
    setPosition(getPosition()+vec);
}

void Camera3D::rotate(const vec3 & vec) {
    setRotation(getRotation()+vec);
}

}