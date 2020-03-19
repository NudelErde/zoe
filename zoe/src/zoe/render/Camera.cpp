//
// Created by florian on 13.03.20.
//

#include "Camera.h"
#include "Material.h"
#include <math.h>

namespace Zoe{


    void Camera::setPosition(const vec3 &position) {
        this->position = position;
        changed = true;
    }

    void Camera::setRotation(const vec3 &rotation) {
        this->rotation = rotation;
        changed = true;
    }

    void Camera::setPitch(const float &pitch) {
        rotation.x = pitch;
        if(rotation.x > 2*PI)
            rotation.x -= 2*PI;
        changed = true;
    }

    void Camera::setYaw(const float &yaw) {
        rotation.y = yaw;
        if(rotation.y > 2*PI)
            rotation.y -= 2*PI;
        changed = true;
    }

    void Camera::setRoll(const float &roll) {
        rotation.z = roll;
        if(rotation.z > 2*PI)
            rotation.z -= 2*PI;
        changed = true;
    }

    void Camera::setRotation(const float &pitch, const float &yaw, const float &roll) {
        rotation.x = pitch;
        rotation.y = yaw;
        rotation.z = roll;
        changed = true;
    }

    void Camera::setDirection(const vec3 &direction) {
        setDirection(direction, 0);
    }

    void Camera::setDirection(const vec3 &direction, const float &roll) {
        vec3 d = direction.normalize();
        setRotation(asin(-d.y),atan2(d.x, d.z),roll);
        changed = true;
    }

    const vec3 &Camera::getPosition() const{
        return position;
    }

    const vec3 &Camera::getRotation() const{
        return rotation;
    }

    vec3 Camera::getDirection() const{
        return (rotateXZ3D(rotation.y) * rotateYZ3D(rotation.x) * rotateXY3D(rotation.z) *(vec4){0,0,-1,0}).xyz;
    }

    const mat4x4 &Camera::getViewMatrix() {
        if(changed){
            changed = false;
            viewMatrix = calcViewMatrix(position, rotation);
        }
        return viewMatrix;
    }

    mat4x4 Camera::getViewMatrix() const {
        if(changed){
            return calcViewMatrix(position, -1 * rotation);
        }else {
            return viewMatrix;
        }
    }

    const mat4x4 &Camera::getProjectionMatrix() const {
        return projectionMatrix;
    }

    void Camera::setProjectionMatrix(const mat4x4 &projectionMatrix) {
        this->projectionMatrix = projectionMatrix * Zoe::scale3D(1,1,-1);
    }

    void Camera::draw(Material material, const Model& model, const std::shared_ptr<Render> &render) {
        material.setProjectionMatrix(getProjectionMatrix());
        material.setViewMatrix(getViewMatrix());
        material.setModelMatrix(model.getModelMatrix());
        material.bind();
        render->draw(*model.getVertexArray(), *material.shader);
    }
}