//
// Created by Florian on 11.07.2020.
//

#pragma once

#include "../core/Core.h"
#include "../math/mat.h"
#include "../math/vec.h"
#include "../render/api/Render.h"

namespace Zoe {

class Model;

class Material;

class DLL_PUBLIC Camera {
public:
    Camera();

    virtual ~Camera();

    void setViewMatrix(const mat4x4 &viewMatrix);

    void setProjectionMatrix(const mat4x4 &projectionMatrix);

    [[nodiscard]] const mat4x4 &getViewMatrix() const;

    [[nodiscard]] const mat4x4 &getProjectionMatrix() const;

    void draw(const Model &model, Material &material) const;

    inline void setRender(const std::shared_ptr<Render>& render){this->render = render;}
    inline const std::shared_ptr<Render>& getRender() const {return render;}

protected:
    mat4x4 viewMatrix;
    mat4x4 projectionMatrix;

    std::shared_ptr<Render> render;
};

class DLL_PUBLIC Camera2D : public Camera {
public:
    Camera2D(const vec2 &position, const float &width, const float &height);

    void setPosition(const vec2 &position);

    [[nodiscard]] const vec2 &getPosition() const;

private:
    vec2 position;
    float width, height;
};

class DLL_PUBLIC Camera3D : public Camera {
public:
    Camera3D(const vec3 &position, const vec3 &rotation, const float& fov, const float& ratio);

    void setPosition(const vec3 &position);

    void setRotation(const vec3 &rotation);

    void setPitch(const float &pitch);

    void setYaw(const float &yaw);

    void setRoll(const float &roll);

    void setRotation(const float &pitch, const float &yaw, const float &roll);

    void setDirection(const vec3 &direction);

    void setDirection(const vec3 &direction, const float &roll);

    void setFoV(const float& fov);

    void setRatio(const float& ratio);

    [[nodiscard]] const vec3 &getPosition() const;

    void move(const vec3&);

    void rotate(const vec3&);

    [[nodiscard]] const vec3 &getRotation() const;

    [[nodiscard]] vec3 getDirection() const;

    [[nodiscard]] inline const float& getFoV() const{return fov;}
    [[nodiscard]] inline const float& getRatio() const{return ratio;}
private:
    vec3 position;
    vec3 rotation;
    float fov, ratio;
};

}