/*
 * VertexBuffer.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/VertexArray.h"

namespace Zoe{

class GraphicsContext;

class OpenGLVertexArrayImpl: public VertexArrayImpl{
public:

	OpenGLVertexArrayImpl(GraphicsContext* context);
	~OpenGLVertexArrayImpl();

	void set(VertexBuffer& vb,IndexBuffer& ib,VertexBufferLayout& layout);

	void bind();
	void unbind();
	IndexBufferImpl* getIndexBuffer();
public:
	unsigned int renderID;
	VertexBufferImpl* vb;
	IndexBufferImpl* ib;
};


}
