/*
 * VertexBuffer.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/VertexArray.h"

namespace Zoe {

class GraphicsContext;

/**
 * Implementation of VertexArray.\n
 * OpenGl should be used if no better implementation is available.\n
 * A VertexArray combines a VertexBuffer, an IndexBuffer and a Layout. VertexBuffer and IndexBuffer are bound to an VertexArray.
 */
class OpenGLVertexArrayImpl : public VertexArrayImpl {
public:

    /**
     * Creates a OpenGLVertexArray
     * @param context the GraphicsContext. Must be OpenGLContext
     */
    OpenGLVertexArrayImpl(GraphicsContext *context);

    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLVertexArrayImpl();

    /**
     * Bind the specified VertexBuffer, IndexBuffer and VertexBufferLayout.
     * @param vb the VertexBuffer
     * @param ib the IndexBuffer
     * @param layout the Layout
     */
    void set(VertexBuffer &vb, IndexBuffer &ib, VertexBufferLayout &layout);

    /**
     * Binds the OpenGLVertexArray if not already bound.
     */
    void bind();

    /**
     * Unbinds the OpenGLVertexArray
     */
    void unbind();

    /**
     * Returns the bound IndexBufferImpl. It is guaranteed to be OpenGLIndexBufferImpl.
     * @return the IndexBufferImpl
     */
    IndexBufferImpl *getIndexBuffer() override;

private:
    friend class OpenGLVertexBufferImpl;
    friend class OpenGLIndexBufferImpl;
    unsigned int renderID;
    VertexBufferImpl *vb;
    IndexBufferImpl *ib;
};


}
