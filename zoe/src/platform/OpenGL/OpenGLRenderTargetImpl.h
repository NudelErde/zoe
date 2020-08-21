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

class OpenGLRenderTargetImpl : public RenderTargetImpl {
public:
    OpenGLRenderTargetImpl(GraphicsContext *context, unsigned int width, unsigned int height);

    ~OpenGLRenderTargetImpl() override;

    void bind() override;

    void unbind() override;

    std::shared_ptr<Texture> getColorAttachment() override;

private:
    unsigned int renderID;
    unsigned int depthAttachmentID;
    unsigned int width, height;
    std::shared_ptr<Texture> colorAttachment;
};

class OpenGLRenderTargetImplDefaultOuput : public RenderTargetImpl {
private:
    OpenGLRenderTargetImplDefaultOuput(GraphicsContext *context);

public:
    ~OpenGLRenderTargetImplDefaultOuput();

    void bind() override;

    void unbind() override;

    std::shared_ptr<Texture> getColorAttachment() override;

    friend class OpenGLContext;
};
}