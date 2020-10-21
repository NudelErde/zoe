/*
 * IndexBuffer.h
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
 * It is used to provide a level of abstraction for IndexBuffer.
 */
class IndexBufferImpl {
public:

    /**
     * Creates a IndexBufferImpl with the specified context, whether this buffer is dynamic and generates an ID.
     * @param context the specified context
     * @param dynamic `true` if this buffer is dynamic
     */
    IndexBufferImpl(GraphicsContext* context, bool dynamic) : context(context), id(GraphicsContext::generateID()),
                                                              dynamic(dynamic) {}

    /**
     * Destructs this object.
     */
    virtual ~IndexBufferImpl() {}

    /**
     * Binds this buffer to the bound VertexArray. See implementation for details.
     */
    virtual void bind() = 0;

    /**
     * Unbinds this buffer from the bound VertexArray. See implementation for details.
     */
    virtual void unbind() = 0;

    /**
     * Copies the indices from `data` to the index buffer.
     * See implementation for details.
     * @param data the index array
     * @param count the size of the index array
     */
    virtual void setData(unsigned int* data, unsigned int count) = 0;

    /**
     * Allocates a new index array and populates it with `count` indexes that start at `offset`. The returned array needs to be deleted!
     * See implementation for details.
     * @param offset start of read data
     * @param count count of read data
     * @returns a new heap allocated array with the requested data. The caller has to delete this array.
     */
    virtual unsigned int* getData(unsigned int offset, unsigned int count) = 0;

    /**
     * Returns the count of indices in this IndexBuffer.
     * See implementation for details.
     * @returns the count of indices in this IndexBuffer
     */
    virtual const unsigned int& getCount() const = 0;

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
 * An IndexBuffer stores the indices of the vertices used for drawing. \n
 * If the index buffer has the indices `0,1,2,2,3,0`, a vertex buffer with 4 vertices can be used to draw a rectangle.
 */
class DLL_PUBLIC IndexBuffer {
public:
    /**
     * Creates a IndexBuffer with the specified implementation.
     * @param impl the specified implementation
     */
    IndexBuffer(IndexBufferImpl* impl) : impl(impl) {}

    /**
     * Binds this buffer to the bound VertexArray.
     */
    inline void bind() { impl->bind(); }

    /**
     * Unbinds this buffer from the bound VertexArray.
     */
    inline void unbind() { impl->unbind(); }

    /**
     * Copies the indices from `data` to the index buffer.
     * @param data the index array
     * @param count the size of the index array
     */
    inline void setData(unsigned int* data, unsigned int count) { impl->setData(data, count); };

    /**
     * Allocates a new index array and populates it with `count` indexes that start at `offset`. The returned array needs to be deleted!
     * @param offset start of read data
     * @param count count of read data
     * @returns a new heap allocated array with the requested data. The caller has to delete this array.
     */
    inline unsigned int* getData(unsigned int offset, unsigned int count) { return impl->getData(offset, count); };

    /**
     * Returns the count of indices in this IndexBuffer.
     * @returns the count of indices in this IndexBuffer
     */
    inline const unsigned int& getCount() const { return impl->getCount(); }

    /**
     * Returns the underlying implementation.
     * @return the implementation
     */
    inline IndexBufferImpl* getImpl() { return &impl; }

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
private:
    ImplPointer <IndexBufferImpl> impl;
};

}
