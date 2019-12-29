/*
 * GraphicsContext.h
 *
 *  Created on: 24.07.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"
#include "../File.h"

#include "api/Render.h"

namespace Zoe {
class IndexBuffer;
class IndexBufferImpl;
class Shader;
class ShaderImpl;
class Texture;
class VertexArray;
class VertexArrayImpl;
class VertexBuffer;
class VertexBufferImpl;
class VertexBufferLayout;
} /* namespace Zoe */

namespace Zoe{

class DLL_PUBLIC GraphicsContext{
public:
	GraphicsContext(): boundIndexBuffer(0), boundRender(0), boundShader(0), boundVertexArray(0), boundVertexBuffer(0){}
	virtual ~GraphicsContext(){};
	virtual void init() = 0;
	virtual void swapBuffers() = 0;

	virtual std::shared_ptr<IndexBuffer> getIndexBuffer() = 0;
	virtual std::shared_ptr<Render> getRender() = 0;
	virtual std::shared_ptr<Shader> getShader(const File& file) = 0;
	virtual std::shared_ptr<VertexArray> getVertexArray() = 0;
	virtual std::shared_ptr<VertexBuffer> getVertexBuffer() = 0;
	virtual std::shared_ptr<VertexBufferLayout> getVertexBufferLayout() = 0;
	virtual std::shared_ptr<Texture> getTexture(unsigned int width, unsigned int height) = 0;
	virtual std::shared_ptr<Texture> getTexture(const File& file) = 0;

	IndexBufferImpl* boundIndexBuffer;
	RenderImpl* boundRender;
	ShaderImpl* boundShader;
	VertexArrayImpl* boundVertexArray;
	VertexBufferImpl* boundVertexBuffer;
};

}


