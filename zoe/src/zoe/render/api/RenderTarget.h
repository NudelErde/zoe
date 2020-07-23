//
// Created by Florian on 09.06.2020.
//

#pragma once

#include "../../zpch.h"

#include "../../core/Core.h"
#include "../../math/mat.h"
#include "../../core/File.h"
#include "Texture.h"
#include "ImplPointer.h"
#include <map>

namespace Zoe {

    class GraphicsContext;

    class RenderTargetImpl {
    public:
        RenderTargetImpl(GraphicsContext *context) : context(context), id(GraphicsContext::generateID()) {}

        virtual ~RenderTargetImpl() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual std::shared_ptr<Texture> getColorAttachment() = 0;

        inline const int& getID() const{ return id;};

    protected:
        GraphicsContext *context;

        int id;
    };

    class DLL_PUBLIC RenderTarget {
    public:
        explicit RenderTarget(RenderTargetImpl *impl) : impl(impl) {}

        ~RenderTarget() {}

        inline void bind() { impl->bind(); }
        inline void unbind() { impl->unbind(); }
        std::shared_ptr<Texture> getColorAttachment() { return impl->getColorAttachment();}

        inline RenderTargetImpl* getImpl(){return &impl;}

        inline const int& getID() const { return impl->getID();}

    private:
        ImplPointer<RenderTargetImpl> impl;
    };

}

