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

/**
 * Implementation of VertexBufferImpl\n
 * OpenGl should be used if no better implementation is available.\n
 * A VertexBuffer stores the vertices used for drawing.
 * @see IndexBuffer
 * @see VertexArray
 */
class OpenGLVertexBufferImpl : public VertexBufferImpl {
public:
    /**
     * Creates a VertexBuffer. Is dynamic if `dynamicBuffer` is set to `true`.
     * @param context the GraphicsContext. Must be OpenGLContext
     * @param dynamicBuffer `true` if the VertexBuffer should be dynamic
     */
    OpenGLVertexBufferImpl(GraphicsContext *context, bool dynamicBuffer);

    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLVertexBufferImpl();

    /**
     * Binds this VertexBuffer if not already bound. The VertexBuffer is not directly bound to the OpenGLContext, but to the bound VertexArray.
     * If no VertexArray is bound then the VertexBuffer is bound to the default VertexArray.
     * If a VertexArray is bound to which this VertexBuffer is bound then it is also bound.
     */
    void bind();

    /**
     * Unbinds this VertexBuffer.
     */
    void unbind();

    /**
     * Copies the vertex data from `data` to the vertex buffer.\n
     * If size is the same as it was on the last call then no new buffer needs to be allocated by OpenGL.
     * @param data the vertex data
     * @param size the size of the vertex data array
     */
    void setData(const void *data, unsigned int size);

    /**
     * Allocates a new buffer and populates it with `size` bytes that start at `offset`. The returned array needs to be deleted!
     * @param offset start of read data
     * @param size size of read data
     * @returns a new heap allocated array with the requested data. The caller has to delete this array.
     */
    void *getData(unsigned int offset, unsigned int size);

    /**
     * Returns the size of the vertex data in this VertexBuffer.
     * @return the size of the vertex data in this VertexBuffer
     */
    unsigned int getSize() const override;

private:
    unsigned int renderID;
    unsigned int size;
};

}
