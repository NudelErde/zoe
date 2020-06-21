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
#include "OpenGLVertexArrayImpl.h"

namespace Zoe {

    OpenGLVertexBufferImpl::OpenGLVertexBufferImpl(GraphicsContext *context, bool dynamicBuffer) : VertexBufferImpl(
            context), dynamicBuffer(dynamicBuffer) {
        glGenBuffers(1, &renderID);
        size = 0;
    }

    OpenGLVertexBufferImpl::~OpenGLVertexBufferImpl() {
        if(context->boundVertexArray != nullptr){
            if (((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb != nullptr &&
                ((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb->getID() == id) {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                ((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb = nullptr;
            }
        }else{
            if(context->boundVertexBuffer != nullptr && context->boundVertexBuffer->getID() == id){
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                context->boundVertexBuffer = nullptr;
            }
        }
        glDeleteBuffers(1, &renderID);
    }

    void OpenGLVertexBufferImpl::bind() {
        if(context->boundVertexArray != nullptr){
            if(((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb == nullptr ||
               ((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb->getID() != id){
                glBindBuffer(GL_ARRAY_BUFFER, renderID);
                ((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb = this;
            }
        }else{
            if(context->boundVertexBuffer == nullptr || context->boundVertexBuffer->getID() != id){
                glBindBuffer(GL_ARRAY_BUFFER, renderID);
                context->boundVertexBuffer = this;
            }
        }
    }

    void OpenGLVertexBufferImpl::unbind() {
        if(context->boundVertexArray != nullptr){
            if(((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb != nullptr){
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                ((OpenGLVertexArrayImpl *) context->boundVertexArray)->vb = nullptr;
            }
        }else{
            if(context->boundVertexBuffer != nullptr){
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                context->boundVertexBuffer = nullptr;
            }
        }
    }

    void OpenGLVertexBufferImpl::setData(const void *data, unsigned int size) {
        bind();
        if (size == this->size) {
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        } else {
            glBufferData(GL_ARRAY_BUFFER, size, data, dynamicBuffer ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
        this->size = size;
    }

    void *OpenGLVertexBufferImpl::getData(unsigned int offset, unsigned int size) {
        if (size == 0) {
            size = this->size;
        }
        void *data = malloc(size);
        bind();
        glGetBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
        return data;
    }

}
