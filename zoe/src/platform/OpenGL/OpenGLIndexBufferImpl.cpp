/*
 * OpenGLIndexBufferImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#include "OpenGLIndexBufferImpl.h"

#include <GL/glew.h>
#include <cstdlib>

#include "../../zoe/render/GraphicsContext.h"
#include "OpenGLVertexArrayImpl.h"

namespace Zoe {

OpenGLIndexBufferImpl::OpenGLIndexBufferImpl(GraphicsContext *context, bool dynamicBuffer) : IndexBufferImpl(context,
                                                                                                             dynamicBuffer) {
    glGenBuffers(1, &renderID);
    count = 0;
}

OpenGLIndexBufferImpl::~OpenGLIndexBufferImpl() {
    if (context->boundVertexArray != nullptr) {
        if (((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib != nullptr &&
            ((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib->getID() == id) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            ((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib = nullptr;
        }
    } else {
        if (context->boundIndexBuffer != nullptr && context->boundIndexBuffer->getID() == id) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            context->boundIndexBuffer = nullptr;
        }
    }
    glDeleteBuffers(1, &renderID);
}

void OpenGLIndexBufferImpl::bind() {
    if (context->boundVertexArray != nullptr) {
        if (((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib == nullptr ||
            ((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib->getID() != id) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID);
            ((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib = this;
        }
    } else {
        if (context->boundIndexBuffer == nullptr || context->boundIndexBuffer->getID() != id) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID);
            context->boundIndexBuffer = this;
        }
    }
}

void OpenGLIndexBufferImpl::unbind() {
    if (context->boundVertexArray != nullptr) {
        if (((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib != nullptr) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            ((OpenGLVertexArrayImpl *) context->boundVertexArray)->ib = nullptr;
        }
    } else {
        if (context->boundIndexBuffer != nullptr) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            context->boundIndexBuffer = nullptr;
        }
    }
}

void OpenGLIndexBufferImpl::setData(const unsigned int *data, unsigned int count) {
    bind();
    if (count == this->count) {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * count, data);
    } else {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data,
                     dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }
    this->count = count;
}

unsigned int *OpenGLIndexBufferImpl::getData(unsigned int offset, unsigned int count) {
    if (count == 0) {
        count = this->count;
    }
    unsigned int size = sizeof(unsigned int) * count;
    void *data = malloc(size);
    bind();
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    return (unsigned int *) data;
}

const unsigned int &OpenGLIndexBufferImpl::getCount() const { return count; }

}
