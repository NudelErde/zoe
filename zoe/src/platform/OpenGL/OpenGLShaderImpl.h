/*
 * OpenGLShaderImpl.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../zoe/File.h"
#include "../zoe/math/mat.h"
#include "../zoe/render/api/Shader.h"
#include <string>
#include <map>

namespace Zoe{

class GraphicsContext;

class OpenGLShaderImpl: public ShaderImpl{
public:
	OpenGLShaderImpl(const File& filepath, GraphicsContext* context);
	~OpenGLShaderImpl();

	void setUniform1f(const std::string& name,float v0);
	void setUniform2f(const std::string& name,float v0,float v1);
	void setUniform3f(const std::string& name,float v0,float v1,float v2);
	void setUniform4f(const std::string& name,float v0,float v1,float v2,float v3);
	void setUniform2m(const std::string& name,const mat2x2& mat);
	void setUniform3m(const std::string& name,const mat3x3& mat);
	void setUniform4m(const std::string& name,const mat4x4& mat);

	void setTexture(const std::string& name,Texture& texture);

	void bind();
	void unbind();
private:
	unsigned int renderID;
	std::map<std::string, unsigned int> samplerSlot;
};

}
