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

/**
 * An abstract RenderTargetImpl.
 */
class RenderTargetImpl {
public:
    /**
     * Creates a RenderTargetImpl with the specified context and generates an id for this object.
     * @param context the specified context
     */
    RenderTargetImpl(GraphicsContext *context) : context(context), id(GraphicsContext::generateID()) {}

    /**
     * A virtual default destructor.
     */
    virtual ~RenderTargetImpl() = default;

    /**
     * Binds the OpenGLRenderTarget if not already bound.
     */
    virtual void bind() = 0;

    /**
     * Unbinds the OpenGLRenderTarget.
     */
    virtual void unbind() = 0;

    /**
     * Returns the color attachment. This is the texture that contains the color output.
     * @returns the color attachment
     */
    virtual std::shared_ptr<Texture> getColorAttachment() = 0;

    /**
     * Returns an intern number used to identify this object.
     * @return the id
     */
    inline const int &getID() const { return id; };

protected:
    GraphicsContext *context;

    int id;
};

/**
* A Render Object contains information about the RenderTarget and selected settings.
*/
class DLL_PUBLIC RenderTarget {
public:
    /**
     * Creates a RenderTarget from a specified RenderTargetImpl
     * @param impl the specified RenderTargetImpl
     */
    explicit RenderTarget(RenderTargetImpl *impl) : impl(impl) {}

    /**
     * Binds the OpenGLRenderTarget if not already bound.
     */
    inline void bind() { impl->bind(); }

    /**
     * Unbinds the OpenGLRenderTarget.
     */
    inline void unbind() { impl->unbind(); }

    /**
     * Returns the color attachment. This is the texture that contains the color output.
     * @returns the color attachment
     */
    std::shared_ptr<Texture> getColorAttachment() { return impl->getColorAttachment(); }

    /**
     * Returns the RenderTargetImpl.
     * @return the RenderTargetImpl
     */
    inline RenderTargetImpl *getImpl() { return &impl; }

    /**
     * Returns an intern number used to identify this object.
     * @return the id
     */
    inline const int &getID() const { return impl->getID(); }

private:
    ImplPointer <RenderTargetImpl> impl;
};

}

