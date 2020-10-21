/*
 * VertexBuffer.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../core/Core.h"
#include "ImplPointer.h"
#include "../GraphicsContext.h"

namespace Zoe {

/**
 * This class should not be used directly.
 * It is used to provide a level of abstraction for VertexBuffer.
 */
class VertexBufferImpl {
public:

    /**
     * Creates a VertexBufferImpl with the specified context, whether this buffer is dynamic and generates an ID.
     * @param context the specified context
     * @param dynamic `true` if this buffer is dynamic
     */
    VertexBufferImpl(GraphicsContext* context, bool dynamic) : context(context), id(GraphicsContext::generateID()),
                                                               dynamic(dynamic) {}
    /**
     * Destructs this object.
     */
    virtual ~VertexBufferImpl() {}

    /**
     * Binds this buffer to the bound VertexArray. See implementation for details.
     */
    virtual void bind() = 0;

    /**
     * Unbinds this buffer from the bound VertexArray. See implementation for details.
     */
    virtual void unbind() = 0;

    /**
     * Copies the vertex data from `data` to the vertex buffer.
     * See implementation for details.
     * @param data the vertex data
     * @param size the size of the vertex data array
     */
    virtual void setData(const void* data, unsigned int size) = 0;

    /**
     * Allocates a new buffer and populates it with `size` bytes that start at `offset`. The returned array needs to be deleted!
     * See implementation for details.
     * @param offset start of read data
     * @param size size of read data
     * @returns a new heap allocated array with the requested data. The caller has to delete this array.
     */
    virtual void* getData(unsigned int offset, unsigned int size) = 0;

    /**
     * Returns the ID of this object. The ID is unique.
     * @return the ID
     */
    inline const int& getID() const { return id; }

    /**
     * Returns `true` if this buffer is dynamic.
     * @return `true` if this buffer is dynamic
     */
    inline bool isDynamic() const { return dynamic; }

    /**
     * Returns the size of the vertex data in this VertexBuffer.
     * @return the size of the vertex data in this VertexBuffer
     */
    virtual unsigned int getSize() const = 0;

protected:
    /**
     * The used graphics context.
     */
    GraphicsContext* context;

    /**
     * A unique id.
     */
    int id;

    /**
     * `true` if this buffer is dynamic.
     */
    bool dynamic;
};

/**
 * A VertexBuffer stores the vertices used for drawing.
 */
class DLL_PUBLIC VertexBuffer {
public:
    /**
     * Creates a VertexBuffer with the specified implementation.
     * @param impl the specified implementation
     */
    VertexBuffer(VertexBufferImpl* impl) : impl(impl) {}

    /**
     * Binds this buffer to the bound VertexArray.
     */
    inline void bind() { impl->bind(); }

    /**
     * Unbinds this buffer from the bound VertexArray.
     */
    inline void unbind() { impl->unbind(); }

    /**
     * Copies the vertex data from `data` to the vertex buffer.
     * @param data the vertex data
     * @param size the size of the vertex data array
     */
    inline void setData(const void* data, unsigned int size) { impl->setData(data, size); }

    /**
     * Allocates a new buffer and populates it with `size` bytes that start at `offset`. The returned array needs to be deleted!
     * @param offset start of read data
     * @param size size of read data
     * @returns a new heap allocated array with the requested data. The caller has to delete this array.
     */
    inline void* getData(unsigned int offset, unsigned int size) { return impl->getData(offset, size); }

    /**
     * Returns the underlying implementation.
     * @return the implementation
     */
    inline VertexBufferImpl* getImpl() { return &impl; }

    /**
     * Returns the ID of this object. The ID is unique for the implementation.
     * @return the ID
     */
    inline const int& getID() const { return impl->getID(); }

    /**
     * Returns `true` if this buffer is dynamic.
     * @return `true` if this buffer is dynamic
     */
    inline bool isDynamic() const { return impl->isDynamic(); }

    /**
     * Returns the size of the vertex data in this VertexBuffer.
     * @return the size of the vertex data in this VertexBuffer
     */
    inline unsigned int getSize() const { return impl->getSize(); }
private:
    ImplPointer<VertexBufferImpl> impl;
};

}
