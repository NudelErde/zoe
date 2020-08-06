/*
 * OpenGLShaderImpl.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/core/File.h"
#include "../../zoe/math/mat.h"
#include "../../zoe/render/api/Shader.h"
#include <string>
#include <map>

namespace Zoe {

class GraphicsContext;

class OpenGLShaderImpl : public ShaderImpl {
public:
    OpenGLShaderImpl(const File &filepath, GraphicsContext *context);

    ~OpenGLShaderImpl() override;

    void setUniform1f(const std::string &name, float v0) override;

    void setUniform2f(const std::string &name, float v0, float v1) override;

    void setUniform3f(const std::string &name, float v0, float v1, float v2) override;

    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) override;

    void setUniform2m(const std::string &name, const mat2x2 &mat) override;

    void setUniform3m(const std::string &name, const mat3x3 &mat) override;

    void setUniform4m(const std::string &name, const mat4x4 &mat) override;

    void setTexture(const std::string &name, Texture &texture) override;

    void bind() override;

    void unbind() override;

    const std::map<std::string, std::string> &getTags() override;

private:

    int getUniformLocation(const std::string&) const;

private:
    unsigned int renderID;
    std::map<std::string, unsigned int> samplerSlot;
    std::map<std::string, std::string> tags;
};

}
