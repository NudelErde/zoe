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

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    unsigned int offset;
};

class OpenGLVertexBufferLayoutImpl : public VertexBufferLayoutImpl {
public:
    OpenGLVertexBufferLayoutImpl();

    ~OpenGLVertexBufferLayoutImpl();

    void push_float(unsigned int count);

    void push_uint(unsigned int count);

    void push_uchar(unsigned int count);

    inline unsigned int getStride() { return stride; }

    inline const std::vector<VertexBufferElement> &getElements() { return elements; }

private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;

};

}
