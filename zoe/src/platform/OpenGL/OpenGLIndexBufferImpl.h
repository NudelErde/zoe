/*
 * OpenGLIndexBufferImpl.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/IndexBuffer.h"

namespace Zoe {

class OpenGLIndexBufferImpl : public IndexBufferImpl {
public:
    OpenGLIndexBufferImpl(GraphicsContext *context, bool dynamicBuffer);

    ~OpenGLIndexBufferImpl();

    void bind();

    void unbind();

    void setData(unsigned int *data, unsigned int count);

    unsigned int *getData(unsigned int offset, unsigned int count);

    const unsigned int &getCount() const;

public:
    unsigned int renderID;
    unsigned int count;
};

}
