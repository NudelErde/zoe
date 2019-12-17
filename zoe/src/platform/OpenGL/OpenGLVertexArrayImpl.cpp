/*
 * OpenGLVertexArrayImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */


#include "OpenGLVertexArrayImpl.h"

#include "OpenGLVertexBufferLayoutImpl.h"
#include "GL/glew.h"

#include "../../zoe/render/GraphicsContext.h"

namespace Zoe{

OpenGLVertexArrayImpl::OpenGLVertexArrayImpl(GraphicsContext* context): VertexArrayImpl(context), vb(0), ib(0){
	glGenVertexArrays(1,&renderID);
}

OpenGLVertexArrayImpl::~OpenGLVertexArrayImpl(){
	glDeleteVertexArrays(1,&renderID);
}

void OpenGLVertexArrayImpl::set(VertexBuffer& vb,IndexBuffer& ib,VertexBufferLayout& layout){
	this->vb = vb.getImpl();
	bind();
	vb.bind();

	ib.bind();

	VertexBufferLayoutImpl* vbl_ptr = layout.getImpl();
	OpenGLVertexBufferLayoutImpl* lvbl = (OpenGLVertexBufferLayoutImpl*)vbl_ptr;
	const auto& elements = lvbl->getElements();
	unsigned int offset = 0;
	for(unsigned int i=0; i<elements.size();i++){
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i,element.count,element.type,element.normalized,lvbl->getStride(),(const void*)(intptr_t)(offset));
		offset += element.count * element.offset;
	}
}

void OpenGLVertexArrayImpl::bind(){
	if(this->context->boundVertexArray != this){
		glBindVertexArray(renderID);
		this->context->boundVertexArray = this;
	}
}

void OpenGLVertexArrayImpl::unbind(){
	if(this->context->boundVertexArray!=0){
		glBindVertexArray(0);
		this->context->boundVertexArray = 0;
	}
}

IndexBufferImpl& OpenGLVertexArrayImpl::getIndexBuffer(){
	return *ib;
}

}
