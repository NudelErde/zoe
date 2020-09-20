/*
 * OpenGLVertexBufferLayoutImpl.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/VertexBufferLayout.h"
#include <vector>

namespace Zoe {

//TODO: rename to unique name
/**
 * Stores data of OpenGLVertexBufferLayoutImpl
 */
struct VertexBufferElement {
    /**
     * Type of the buffer element
     */
    unsigned int type;
    /**
     * Size of the buffer element. Values greater than 1 are vectors.
     */
    unsigned int count;
    /**
     * GL_TRUE or if data should be normalized. GL_FALSE otherwise.
     */
    unsigned char normalized;
    /**
     * Offset of this buffer element.
     */
    unsigned int offset;
};

/**
 * Implementation of VertexBufferLayout.\n
 * OpenGl should be used if no better implementation is available.\n
 * A VertexBufferLayout stores how the data in a VertexBuffer should be interpreted.
 */
class OpenGLVertexBufferLayoutImpl : public VertexBufferLayoutImpl {
public:
    /**
     * Creates a OpenGLVertexBufferLayout
     */
    OpenGLVertexBufferLayoutImpl();

    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLVertexBufferLayoutImpl();

    /**
     * Push `count` floats on the layout.
     * @param count count of floats
     */
    void push_float(unsigned int count);

    /**
     * Push `count` unsigned ints on the layout.
     * @param count count of unsigned ints
     */
    void push_uint(unsigned int count);

    /**
     * Push `count` unsigned chars on the layout.
     * @param count count of unsigned chars
     */
    void push_uchar(unsigned int count);

    /**
     * Returns stride/size of this layout.
     * @return stride of this layout.
     */
    inline unsigned int getStride() { return stride; }

    /**
     * Return the stored VertexBufferElements.
     * @return the stored VertexBufferElements
     */
    inline const std::vector<VertexBufferElement> &getElements() { return elements; }

private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;

};

}
