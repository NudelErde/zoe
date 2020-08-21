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

class VertexArrayImpl {
public:

    VertexArrayImpl(GraphicsContext *context) : context(context), id(GraphicsContext::generateID()) {}

    virtual ~VertexArrayImpl() {}

    virtual void set(VertexBuffer &vb, IndexBuffer &ib, VertexBufferLayout &layout) = 0;

    virtual void bind() = 0;

    virtual void unbind() = 0;

    virtual IndexBufferImpl *getIndexBuffer() = 0;

    inline const int &getID() const { return id; }

protected:
    GraphicsContext *context;

    int id;
};

class DLL_PUBLIC VertexArray {
public:

    VertexArray(VertexArrayImpl *impl) : impl(impl) {}

    ~VertexArray() {}

    //TODO: signature changed, correct all method calls
    inline void set(std::shared_ptr<VertexBuffer> vb, std::shared_ptr<IndexBuffer> ib, std::shared_ptr<VertexBufferLayout> layout) {
        impl->set(*vb, *ib, *layout);
        bufferLayout = layout;
    }

    inline void bind() { impl->bind(); }

    inline void unbind() { impl->unbind(); }

    inline VertexArrayImpl *getImpl() { return &impl; }

    inline const int &getID() const { return impl->getID(); }

    inline const std::shared_ptr<VertexBufferLayout> getVertexBufferLayout() const {return bufferLayout;}

private:
    ImplPointer<VertexArrayImpl> impl;
    std::shared_ptr<VertexBufferLayout> bufferLayout;
};

}
