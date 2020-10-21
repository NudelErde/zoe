//
// Created by Florian on 09.06.2020.
//

#pragma once

#include "../../zoe/render/api/RenderTarget.h"
#include "../../zoe/math/vec.h"
#include "../../zoe/render/api/Texture.h"
#include <GL/glew.h>

namespace Zoe {

class GraphicsContext;

/**
 * Implementation of RenderTarget.\n
 * OpenGl should be used if no better implementation is available.\n
 * A RenderTarget can be the target of draw calls. A RenderTarget can be bound to a Render object.\n
 * An OpenGLRenderTarget has an RGBA Texture and a Depth RenderBuffer.\n
 * The RGBA Texture can be accessed via the OpenGLRenderTargetImpl::getColorAttachment method.
 */
class OpenGLRenderTargetImpl : public RenderTargetImpl {
public:
    /**
     * Creates an OpenGLRenderTarget with a specified width and height.
     * @param context the GraphicsContext. Must be OpenGLContext
     * @param width width of the OpenGLRenderTarget
     * @param height height of the OpenGLRenderTarget
     */
    OpenGLRenderTargetImpl(GraphicsContext *context, unsigned int width, unsigned int height);

    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLRenderTargetImpl() override;

    /**
     * Binds the OpenGLRenderTarget if not already bound.
     */
    void bind() override;

    /**
     * Unbinds the OpenGLRenderTarget
     */
    void unbind() override;

    /**
     * Returns the color attachment. This is the texture that contains the color output.
     * @returns the color attachment
     */
    std::shared_ptr<Texture> getColorAttachment() override;

private:
    unsigned int renderID;
    unsigned int depthAttachmentID;
    unsigned int width, height;
    std::shared_ptr<Texture> colorAttachment;
};

/**
 * Implementation of RenderTarget.\n
 * OpenGl should be used if no better implementation is available.\n
 * The OpenGLRenderTargetImplDefaultOutput is the default render target.\n
 * The target of draw calls with this RenderTarget is the window buffer.
 */
class OpenGLRenderTargetImplDefaultOuput : public RenderTargetImpl {
private:

    /**
     * Creates the default RenderTarget instance should only be used by OpenGLContextImpl
     * @param context the GraphicsContext. Must be OpenGLContext
     */
    OpenGLRenderTargetImplDefaultOuput(GraphicsContext *context);

public:
    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLRenderTargetImplDefaultOuput();

    /**
     * Binds the OpenGLRenderTarget if not already bound.
     */
    void bind() override;

    /**
     * Unbinds the OpenGLRenderTarget
     */
    void unbind() override;

    /**
     * Should not be used. Can not get ColorAttachment of Window Buffer.
     * @returns nullptr
     */
    std::shared_ptr<Texture> getColorAttachment() override;

    friend class OpenGLContext;
};
}