/*
 * VertexArray.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "ImplPointer.h"
#include "../GraphicsContext.h"

namespace Zoe {

class GraphicsContext;

/**
 * This class should not be used directly.
 * It is used to provide a level of abstraction for VertexArray.
 */
class VertexArrayImpl {
public:

    /**
     * Creates a VertexArrayImpl with the specified context and generates an ID.
     * @param context the specified context
     */
    VertexArrayImpl(GraphicsContext* context) : context(context), id(GraphicsContext::generateID()) {}

    /**
     * Destructs this object.
     */
    virtual ~VertexArrayImpl() {}

    /**
     * Bind the specified VertexBuffer, IndexBuffer and VertexBufferLayout.
     * See implementation for details.
     * @param vb the VertexBuffer
     * @param ib the IndexBuffer
     * @param layout the Layout
     */
    virtual void set(VertexBuffer& vb, IndexBuffer& ib, VertexBufferLayout& layout) = 0;

    /**
     * Binds this VertexArray. See implementation for details.
     */
    virtual void bind() = 0;

    /**
     * Unbinds this VertexArray. See implementation for details.
     */
    virtual void unbind() = 0;

    /**
     * Returns the bound IndexBufferImpl.
     * It should be from the same implementation as this object is (OpenGLVertexArrayImpl should return a OpenGLIndexBufferImpl).
     * See implementation for details.
     * @return the bound IndexBufferImpl
     */
    virtual IndexBufferImpl* getIndexBuffer() = 0;

    /**
     * Returns the ID of this object. The ID is unique.
     * @return the ID
     */
    inline const int& getID() const { return id; }

protected:

    /**
     * The used graphics context.
     */
    GraphicsContext* context;

    /**
     * A unique id.
     */
    int id;
};

/**
 * A VertexArray combines a VertexBuffer, an IndexBuffer and a Layout. VertexBuffer and IndexBuffer are bound to an VertexArray.
 */
class DLL_PUBLIC VertexArray {
public:

    /**
     * Creates a VertexArray with the specified implementation.
     * @param impl the specified implementation
     */
    VertexArray(VertexArrayImpl* impl) : impl(impl) {}

    /**
     * Bind the specified VertexBuffer, IndexBuffer and VertexBufferLayout.
     * @param vb the VertexBuffer
     * @param ib the IndexBuffer
     * @param layout the Layout
     */
    inline void
    set(std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<VertexBufferLayout> layout) {
        impl->set(*vb, *ib, *layout);
        bufferLayout = layout;
    }

    /**
     * Binds this VertexArray.
     */
    inline void bind() { impl->bind(); }

    /**
     * Unbinds this VertexArray.
     */
    inline void unbind() { impl->unbind(); }

    /**
     * Returns the underlying implementation.
     * @return the implementation
     */
    inline VertexArrayImpl* getImpl() { return &impl; }

    /**
     * Returns the ID of this object. The ID is unique for the implementation.
     * @return the ID
     */
    inline const int& getID() const { return impl->getID(); }

    /**
     * Returns the VertexBufferLayout that was used in VertexArray::set.
     * @return the VertexBufferLayout
     */
    inline const std::shared_ptr<VertexBufferLayout> getVertexBufferLayout() const { return bufferLayout; }

private:
    ImplPointer<VertexArrayImpl> impl;
    std::shared_ptr<VertexBufferLayout> bufferLayout;
};

}
