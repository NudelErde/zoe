/*
 * Shader.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zpch.h"

#include "../../Core.h"
#include "../../math/mat.h"
#include "../../File.h"
#include "Texture.h"
#include "ImplPointer.h"
#include <map>

namespace Zoe {

class GraphicsContext;

class ShaderImpl{
public:
	ShaderImpl(GraphicsContext* context):context(context){}
	virtual ~ShaderImpl() = default;

	virtual void setUniform1f(const std::string& name,float v0)=0;
	virtual void setUniform2f(const std::string& name,float v0,float v1)=0;
	virtual void setUniform3f(const std::string& name,float v0,float v1,float v2)=0;
	virtual void setUniform4f(const std::string& name,float v0,float v1,float v2,float v3)=0;
	virtual void setUniform2m(const std::string& name,const mat2x2& mat)=0;
	virtual void setUniform3m(const std::string& name,const mat3x3& mat)=0;
	virtual void setUniform4m(const std::string& name,const mat4x4& mat)=0;

	virtual void setTexture(const std::string& name, Texture& texture)=0;

	virtual void bind()=0;
	virtual void unbind()=0;

    virtual const std::map<std::string, std::string>& getTags()=0;

protected:
	GraphicsContext* context;
};

class DLL_PUBLIC Shader {
public:
	explicit Shader(ShaderImpl* impl): impl(impl) {}
	~Shader() {}

	inline void setUniform1f(const std::string& name, float v0){ impl->setUniform1f(name, v0); }
	inline void setUniform2f(const std::string& name, float v0, float v1){ impl->setUniform2f(name, v0, v1); }
	inline void setUniform3f(const std::string& name, float v0, float v1,float v2){ impl->setUniform3f(name, v0, v1, v2); }
	inline void setUniform4f(const std::string& name, float v0, float v1,float v2, float v3){ impl->setUniform4f(name, v0, v1, v2, v3); }
	inline void setUniform2m(const std::string& name,const mat2x2& mat){impl->setUniform2m(name, mat); }
	inline void setUniform3m(const std::string& name,const mat3x3& mat){impl->setUniform3m(name, mat); }
	inline void setUniform4m(const std::string& name,const mat4x4& mat){impl->setUniform4m(name, mat); }

	inline void setTexture(const std::string& name, Texture& texture){impl->setTexture(name, texture);}

	inline void bind() { impl->bind(); }
	inline void unbind(){ impl->unbind(); }

	inline const std::map<std::string, std::string>& getTags(){return impl->getTags();}

	inline ShaderImpl* getImpl() {return &impl;}

private:
	ImplPointer<ShaderImpl> impl;
};

}
