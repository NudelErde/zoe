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

	virtual IndexBuffer* getIndexBuffer() = 0;
	virtual Render* getRender() = 0;
	virtual Shader* getShader(const File& file) = 0;
	virtual VertexArray* getVertexArray() = 0;
	virtual VertexBuffer* getVertexBuffer() = 0;
	virtual VertexBufferLayout* getVertexBufferLayout() = 0;
	virtual Texture* getTexture(unsigned int width, unsigned int height) = 0;
	virtual Texture* getTexture(const File& file) = 0;

	IndexBufferImpl* boundIndexBuffer;
	RenderImpl* boundRender;
	ShaderImpl* boundShader;
	VertexArrayImpl* boundVertexArray;
	VertexBufferImpl* boundVertexBuffer;
};

}


