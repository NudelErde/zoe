/*
 * OpenGLVertexBufferImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */


#include "OpenGLVertexBufferImpl.h"

#include <GL/glew.h>
#include <stdlib.h>

#include "../../zoe/render/GraphicsContext.h"

namespace Zoe{

OpenGLVertexBufferImpl::OpenGLVertexBufferImpl(GraphicsContext* context): VertexBufferImpl(context) {
	glGenBuffers(1, &renderID);
	size = 0;
}

OpenGLVertexBufferImpl::~OpenGLVertexBufferImpl() {
	glDeleteBuffers(1,&renderID);
}

void OpenGLVertexBufferImpl::bind(){
	if(this->context->boundVertexBuffer != this){
		glBindBuffer(GL_ARRAY_BUFFER, renderID);
		this->context->boundVertexBuffer = this;
	}
}

void OpenGLVertexBufferImpl::unbind(){
	if(this->context->boundVertexBuffer!=0){
		glBindBuffer(GL_ARRAY_BUFFER,0);
		this->context->boundVertexBuffer = 0;
	}
}

void OpenGLVertexBufferImpl::setData(const void* data,unsigned int size){
	bind();
	if(size == this->size){
		glBufferSubData(GL_ARRAY_BUFFER,0,size,data);
	}else{
		glBufferData(GL_ARRAY_BUFFER,size,data, GL_STATIC_DRAW);
	}
	this->size=size;
}
void* OpenGLVertexBufferImpl::getData(unsigned int offset,unsigned int size){
	if(size == 0){
		size = this->size;
	}
	void* data = malloc(size);
	bind();
	glGetBufferSubData(GL_ARRAY_BUFFER,offset,size,data);
	return data;
}

}
