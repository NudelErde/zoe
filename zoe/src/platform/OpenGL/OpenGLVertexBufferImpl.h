/*
 * OpenGLVertexBufferImpl.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/VertexBuffer.h"

namespace Zoe {

class GraphicsContext;

class OpenGLVertexBufferImpl: public VertexBufferImpl {
public:
	OpenGLVertexBufferImpl(GraphicsContext* context, bool dynamicBuffer);
	~OpenGLVertexBufferImpl();

	void bind();
	void unbind();

	void setData(const void* data, unsigned int size);
	void* getData(unsigned int offset, unsigned int size);
private:
    bool dynamicBuffer;
    unsigned int renderID;
    unsigned int size;
};

}
