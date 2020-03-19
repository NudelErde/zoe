//
// Created by florian on 13.03.20.
//

#pragma once

#include "../Core.h"
#include "../math/mat.h"
#include "../math/vec.h"
#include "api/VertexArray.h"
#include "api/Render.h"
#include "Model.h"

namespace Zoe{

    class Material;

    class DLL_PUBLIC Camera {
    public:

        void setPosition(const vec3& position);
        void setRotation(const vec3& rotation);
        void setPitch(const float& pitch);
        void setYaw(const float& yaw);
        void setRoll(const float& roll);
        void setRotation(const float& pitch, const float& yaw, const float& roll);
        void setDirection(const vec3& direction);
        void setDirection(const vec3& direction, const float& roll);

        const vec3& getPosition() const;
        const vec3& getRotation() const;
        vec3 getDirection() const;

        void setProjectionMatrix(const mat4x4& projectionMatrix);

        const mat4x4& getViewMatrix();
        mat4x4 getViewMatrix() const;
        const mat4x4& getProjectionMatrix() const;

        void draw(Material material, const Model& model, const std::shared_ptr<Render>& render);
    private:
        vec3 position;
        vec3 rotation;

        mat4x4 viewMatrix;
        mat4x4 projectionMatrix;
        bool changed = false;
    };

}

