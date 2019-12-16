/*
 * OpenGLVertexBufferLayoutImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#include "OpenGLVertexBufferLayoutImpl.h"

#include "GL/glew.h"

namespace Zoe {

OpenGLVertexBufferLayoutImpl::OpenGLVertexBufferLayoutImpl() :
		stride(0) {
}
OpenGLVertexBufferLayoutImpl::~OpenGLVertexBufferLayoutImpl() {
}

void OpenGLVertexBufferLayoutImpl::push_float(unsigned int count) {
	VertexBufferElement vbe = { GL_FLOAT, count,
	GL_FALSE, sizeof(float) };
	elements.push_back(vbe);
	stride += count * sizeof(float);
}

void OpenGLVertexBufferLayoutImpl::push_uint(unsigned int count) {
	VertexBufferElement vbe = { GL_UNSIGNED_INT, count,
	GL_FALSE, sizeof(unsigned int) };
	elements.push_back(vbe);
	stride += count * sizeof(unsigned int);
}

void OpenGLVertexBufferLayoutImpl::push_uchar(unsigned int count) {
	VertexBufferElement vbe = { GL_UNSIGNED_BYTE, count,
	GL_TRUE, sizeof(unsigned char) };
	elements.push_back(vbe);
	stride += count * sizeof(unsigned char);
}

}
