/*
 * Material.h
 *
 *  Created on: 25.06.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"
#include "../math/mat.h"
#include "../render/api/Shader.h"
#include "api/VertexBufferLayout.h"
#include "Camera.h"

namespace Zoe {

    class VertexArray;
    class Render;

    class DLL_PUBLIC Material {
    public:

        Material();

        Material(const File &materialSource, std::shared_ptr<VertexBufferLayout> bufferLayout);

        Material(std::shared_ptr<Shader> shader, std::shared_ptr<VertexBufferLayout> bufferLayout);

        void setModelMatrix(const mat4x4 &modelMatrix);

        void setViewMatrix(const mat4x4 &viewMatrix);

        void setProjectionMatrix(const mat4x4 &projectionMatrix);

        void setUniform(const std::string &name, float x);

        void setUniform(const std::string &name, vec2 vec);

        void setUniform(const std::string &name, vec3 vec);

        void setUniform(const std::string &name, vec4 vec);

        void setUniform(const std::string &name, const mat3x3 &mat);

        void setUniform(const std::string &name, const mat4x4 &mat);

        const VertexBufferLayout &getLayoutBuffer() const;

        void bind();

    private:
        std::shared_ptr<Shader> shader;
        std::shared_ptr<VertexBufferLayout> layout;
        mat4x4 modelMatrix;
        mat4x4 viewMatrix;
        mat4x4 projectionMatrix;
        std::map<std::string, std::function<void(Material *, const std::string &)>> uniformSetter;
        std::map<std::string, std::string> uniformMatrixMap;

        friend void Camera::draw(Zoe::Material material, const Model& model,const std::shared_ptr<Render> &render);

    private:
        void loadTags();
    };

}
