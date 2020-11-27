/*
 * OpenGLIndexBufferImpl.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/IndexBuffer.h"

namespace Zoe {

/**
 * Implementation of IndexBufferImpl\n
 * OpenGl should be used if no better implementation is available.\n
 * An IndexBuffer stores the indices of the vertices used for drawing. \n
 * If the index buffer has the indices `0,1,2,2,3,0`, a vertex buffer with 4 vertices can be used to draw a rectangle.
 * @see VertexBuffer
 * @see VertexArray
 */
class OpenGLIndexBufferImpl : public IndexBufferImpl {
public:
    /**
     * Creates an IndexBuffer. Is dynamic if `dynamicBuffer` is set to `true`.
     * @param context the GraphicsContext. Must be OpenGLContext
     * @param dynamicBuffer `true` if the IndexBuffer should be dynamic
     */
    OpenGLIndexBufferImpl(GraphicsContext *context, bool dynamicBuffer);

    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLIndexBufferImpl();

    /**
     * Binds this IndexBuffer if not already bound. The IndexBuffer is not directly bound to the OpenGLContext, but to the bound VertexArray.
     * If no VertexArray is bound then the IndexBuffer is bound to the default VertexArray.
     * If a VertexArray is bound to which this IndexBuffer is bound then it is also bound.
     */
    void bind();

    /**
     * Unbinds this IndexBuffer.
     */
    void unbind();

    /**
     * Copies the indices from `data` to the index buffer.\n
     * If count is the same as it was on the last call then no new buffer needs to be allocated by OpenGL.
     * @param data the index array
     * @param count the size of the index array
     */
    void setData(const unsigned int *data, unsigned int count);

    /**
     * Allocates a new index array and populates it with `count` indexes that start at `offset`. The returned array needs to be deleted!
     * @param offset start of read data
     * @param count count of read data
     * @returns a new heap allocated array with the requested data. The caller has to delete this array.
     */
    unsigned int *getData(unsigned int offset, unsigned int count);

    /**
     * Returns the count of indices in this IndexBuffer.
     * @returns the count of indices in this IndexBuffer
     */
    const unsigned int &getCount() const;

private:
    unsigned int renderID;
    unsigned int count;
};

}
