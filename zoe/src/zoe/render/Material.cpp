//
// Created by florian on 14.03.20.
//

#include "Material.h"

#include <utility>
#include "GraphicsContext.h"
#include "../Application.h"

namespace Zoe {


    Material::Material(const File &materialSource) : shader(
            Application::getContext().getShader(materialSource)) {
        loadTags();
    }

    Material::Material(std::shared_ptr<Shader> shader) : shader(std::move(shader)) {
        loadTags();
    }

    void Material::setModelMatrix(const mat4x4 &modelMatrix) {
        this->modelMatrix = modelMatrix;
    }

    void Material::setViewMatrix(const mat4x4 &viewMatrix) {
        this->viewMatrix = viewMatrix;
    }

    void Material::setProjectionMatrix(const mat4x4 &projectionMatrix) {
        this->projectionMatrix = projectionMatrix;
    }

    void Material::setUniform(const std::string &name, float x) {
        shader->setUniform1f(name, x);
    }

    void Material::setUniform(const std::string &name, vec2 vec) {
        shader->setUniform2f(name, vec.x, vec.y);
    }

    void Material::setUniform(const std::string &name, vec3 vec) {
        shader->setUniform3f(name, vec.x, vec.y, vec.z);
    }

    void Material::setUniform(const std::string &name, vec4 vec) {
        shader->setUniform4f(name, vec.x, vec.y, vec.z, vec.w);
    }

    void Material::setUniform(const std::string &name, const mat3x3 &mat) {
        shader->setUniform3m(name, mat);
    }

    void Material::setUniform(const std::string &name, const mat4x4 &mat) {
        shader->setUniform4m(name, mat);
    }

    void Material::bind() {
        shader->bind();
        std::string name = uniformMatrixMap["model"];
        uniformSetter[name](this, name);
        if (name != uniformMatrixMap["view"]) {
            name = uniformMatrixMap["view"];
            uniformSetter[name](this, name);
        }
        if (name != uniformMatrixMap["projection"]) {
            name = uniformMatrixMap["projection"];
            uniformSetter[name](this, name);
        }
    }

    void Material::loadTags() {
        render = Application::getContext().getRender();
        std::map<std::string, std::string> tags = shader->getTags();
        if (!tags["modelMatrix"].empty()) {
            uniformSetter[tags["modelMatrix"]] = [](Material *me, const std::string &name) {
                me->setUniform(name, me->modelMatrix);
            };
            uniformMatrixMap["model"] = tags["modelMatrix"];
        } else if (!tags["modelViewMatrix"].empty()) {
            uniformSetter[tags["modelViewMatrix"]] = [](Material *me, const std::string &name) {
                me->setUniform(name, me->modelMatrix * me->viewMatrix);
            };
            uniformMatrixMap["model"] = tags["modelViewMatrix"];
            uniformMatrixMap["view"] = tags["modelViewMatrix"];
        } else if (!tags["modelViewProjectionMatrix"].empty()) {
            uniformSetter[tags["modelViewProjectionMatrix"]] = [](Material *me, const std::string &name) {
                me->setUniform(name, me->modelMatrix * me->viewMatrix * me->projectionMatrix);
            };
            uniformMatrixMap["model"] = tags["modelViewProjectionMatrix"];
            uniformMatrixMap["view"] = tags["modelViewProjectionMatrix"];
            uniformMatrixMap["projection"] = tags["modelViewProjectionMatrix"];
        }

        if (!tags["viewMatrix"].empty()) {
            uniformSetter[tags["viewMatrix"]] = [](Material *me, const std::string &name) {
                me->setUniform(name, me->viewMatrix);
            };
            uniformMatrixMap["view"] = tags["viewMatrix"];
        } else if (!tags["viewProjectionMatrix"].empty()) {
            uniformSetter[tags["viewProjectionMatrix"]] = [](Material *me, const std::string &name) {
                me->setUniform(name, me->viewMatrix * me->projectionMatrix);
            };
            uniformMatrixMap["view"] = tags["viewProjectionMatrix"];
            uniformMatrixMap["projection"] = tags["viewProjectionMatrix"];
        }

        if (!tags["projectionMatrix"].empty()) {
            uniformSetter[tags["projectionMatrix"]] = [](Material *me, const std::string &name) {
                me->setUniform(name, me->projectionMatrix);
            };
            uniformMatrixMap["projection"] = tags["projectionMatrix"];
        }
    }

    Material::Material() = default;
}