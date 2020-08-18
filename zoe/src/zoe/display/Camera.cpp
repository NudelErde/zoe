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
}

const mat4x4 &Camera::getViewMatrix() const {
    return viewMatrix;
}

void Camera::draw(const Model &model, Material &material) const {
    //TODO
    /*material.setModelMatrix(model.getModelMatrix());
    material.setViewMatrix(getViewMatrix());
    material.setProjectionMatrix(getProjectionMatrix()*Zoe::scale3D(1, 1, -1));
    material.bind();
    render->draw(*model.getVertexArray(), *material.getShader());*/
}

Camera::~Camera() = default;

Camera::Camera() = default;

//Camera2D

Camera2D::Camera2D(const vec2 &position, const float &width, const float &height) : position(position), width(width), height(height) {
    projectionMatrix = scale3D(2/width, 2/height, 1);
    viewMatrix = translate3D(-position.x, -position.y, 0);
}

void Camera2D::setPosition(const vec2 &pos) {
    position = pos;
    viewMatrix = translate3D(-pos.x, -pos.y, 0);
}

const vec2 &Camera2D::getPosition() const {
    return position;
}

//Camera3D

Camera3D::Camera3D(const vec3 &pos, const vec3 &rot, const float &fov, const float &ratio) : position(pos),
                                                                                             rotation(rot), fov(fov),
                                                                                             ratio(ratio) {
    projectionMatrix = perspective(0.1f, 100, fov, ratio);
    viewMatrix = calcViewMatrix(pos, -1 * rot);
}

void Camera3D::setPosition(const vec3 &pos) {
    position = pos;
    viewMatrix = calcViewMatrix(pos, -1 * rotation);
}

void Camera3D::setRotation(const vec3 &rot) {
    rotation = rot;
    viewMatrix = calcViewMatrix(position, -1 * rot);
}

void Camera3D::setPitch(const float &pitch) {
    rotation.x = pitch;
    if (rotation.x > 2 * PI)
        rotation.x -= 2 * PI;
    viewMatrix = calcViewMatrix(position, -1 * rotation);
}

void Camera3D::setYaw(const float &yaw) {
    rotation.y = yaw;
    if (rotation.y > 2 * PI)
        rotation.y -= 2 * PI;
    viewMatrix = calcViewMatrix(position, -1 * rotation);
}

void Camera3D::setRoll(const float &roll) {
    rotation.z = roll;
    if (rotation.z > 2 * PI)
        rotation.z -= 2 * PI;
    viewMatrix = calcViewMatrix(position, -1 * rotation);
}

void Camera3D::setRotation(const float &pitch, const float &yaw, const float &roll) {
    rotation.x = pitch;
    rotation.y = yaw;
    rotation.z = roll;
    viewMatrix = calcViewMatrix(position, -1 * rotation);
}

void Camera3D::setDirection(const vec3 &direction) {
    setDirection(direction, 0);
}

void Camera3D::setDirection(const vec3 &direction, const float &roll) {
    vec3 d = direction.normalize();
    setRotation(asin(-d.y), atan2(d.x, d.z), roll);
    viewMatrix = calcViewMatrix(position, -1 * rotation);
}

const vec3 &Camera3D::getPosition() const {
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
    projectionMatrix = perspective(0.01f, 100, fov, ratio);
}

void Camera3D::setRatio(const float &rat) {
    ratio = rat;
    projectionMatrix = perspective(0.01f, 100, fov, ratio);
}

void Camera3D::move(const vec3 & vec) {
    setPosition(getPosition()+vec);
}

void Camera3D::rotate(const vec3 & vec) {
    setRotation(getRotation()+vec);
}

}