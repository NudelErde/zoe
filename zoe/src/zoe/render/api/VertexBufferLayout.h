/*
 * VertexBufferLayout.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "VertexBuffer.h"
#include "ImplPointer.h"
#include "../GraphicsContext.h"

namespace Zoe {

/**
 * This class should not be used directly.
 * It is used to provide a level of abstraction for VertexBufferLayout.
 */
class VertexBufferLayoutImpl {
public:

    /**
     * Creates a VertexBufferLayoutImpl and generates an ID.
     */
    VertexBufferLayoutImpl() : id(GraphicsContext::generateID()) {}
    /**
     * Destructs this object.
     */
    virtual ~VertexBufferLayoutImpl() {}

    /**
     * Push `count` floats on the layout.
     * @param count count of floats
     */
    virtual void push_float(unsigned int count) = 0;

    /**
     * Push `count` unsigned ints on the layout.
     * @param count count of unsigned ints
     */
    virtual void push_uint(unsigned int count) = 0;

    /**
     * Push `count` unsigned chars on the layout.
     * @param count count of unsigned chars
     */
    virtual void push_uchar(unsigned int count) = 0;

    /**
     * Returns the ID of this object. The ID is unique.
     * @return the ID
     */
    inline const int& getID() const { return id; };

private:
    int id;
};

/**
 * A VertexBufferLayout stores how the data in a VertexBuffer should be interpreted.
 */
class DLL_PUBLIC VertexBufferLayout {
public:
    /**
     * Creates a VertexBufferLayout with the specified implementation.
     * @param impl the specified implementation
     */
    VertexBufferLayout(VertexBufferLayoutImpl* impl) : impl(impl) {}

    /**
     * Push `count` floats on the layout.
     * @param count count of floats
     */
    inline void push_float(unsigned int count) { impl->push_float(count); }

    /**
     * Push `count` unsigned ints on the layout.
     * @param count count of unsigned ints
     */
    inline void push_uint(unsigned int count) { impl->push_uint(count); }

    /**
     * Push `count` unsigned chars on the layout.
     * @param count count of unsigned chars
     */
    inline void push_uchar(unsigned int count) { impl->push_uchar(count); }

    /**
     * Returns the underlying implementation.
     * @return the implementation
     */
    inline VertexBufferLayoutImpl* getImpl() { return &impl; }

    /**
     * Returns the ID of this object. The ID is unique for the implementation.
     * @return the ID
     */
    inline const int& getID() const { return impl->getID(); }
private:
    ImplPointer <VertexBufferLayoutImpl> impl;
};

}
