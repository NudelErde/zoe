/*
 * OpenGLIndexBufferImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#include "OpenGLIndexBufferImpl.h"

#include <GL/glew.h>
#include <stdlib.h>

#include "../../zoe/render/GraphicsContext.h"
#include "OpenGLVertexArrayImpl.h"

namespace Zoe{

OpenGLIndexBufferImpl::OpenGLIndexBufferImpl(GraphicsContext* context): IndexBufferImpl(context) {
	glGenBuffers(1, &renderID);
	count = 0;
}

OpenGLIndexBufferImpl::~OpenGLIndexBufferImpl() {
	glDeleteBuffers(1,&renderID);
}

void OpenGLIndexBufferImpl::bind(){
	if(this->context->boundVertexArray){
		if(((OpenGLVertexArrayImpl*)this->context->boundVertexArray)->ib!=this){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID);
			((OpenGLVertexArrayImpl*)this->context->boundVertexArray)->ib = this;
		}
	}else{
		if(this->context->boundIndexBuffer != this){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID);
			this->context->boundIndexBuffer = this;
		}
	}
}

void OpenGLIndexBufferImpl::unbind(){
	if(this->context->boundIndexBuffer != 0){
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		this->context->boundIndexBuffer = 0;
	}
}

void OpenGLIndexBufferImpl::setData(unsigned int* data,unsigned int count){
	bind();
	if(count == this->count){
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,sizeof(unsigned int)*count,data);
	}else{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*count,data,GL_STATIC_DRAW);
	}
	this->count=count;
}
unsigned int* OpenGLIndexBufferImpl::getData(unsigned int offset,unsigned int count){
	if(count == 0){
		count = this->count;
	}
	unsigned int size = sizeof(unsigned int)*count;
	void* data = malloc(size);
	bind();
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER,offset,size,data);
	return (unsigned int*)data;
}

const unsigned int& OpenGLIndexBufferImpl::getCount() const{return count;}

}
