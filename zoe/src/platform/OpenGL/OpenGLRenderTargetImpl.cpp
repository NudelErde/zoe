//
// Created by Florian on 09.06.2020.
//

#include "OpenGLRenderTargetImpl.h"
#include "../../zoe/render/GraphicsContext.h"
#include "OpenGLTextureImpl.h"
#include "../../zoe/core/Application.h"

namespace Zoe {

    OpenGLRenderTargetImpl::OpenGLRenderTargetImpl(Zoe::GraphicsContext *context, unsigned int width,
                                                   unsigned int height) : RenderTargetImpl(context), width(width),
                                                                          height(height) {
        colorAttachment = context->getTexture(width, height, 4);

        glGenRenderbuffers(1, &depthAttachmentID);
        glBindRenderbuffer(GL_RENDERBUFFER, depthAttachmentID);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glGenFramebuffers(1, &renderID);
        glBindFramebuffer(GL_FRAMEBUFFER, renderID);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               ((OpenGLTextureImpl *) (colorAttachment->getImpl()))->getHandle(), 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthAttachmentID);

        unsigned int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            warning("Could not create OpenGL Framebuffer: ", status);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    OpenGLRenderTargetImpl::~OpenGLRenderTargetImpl() {
        if(context->boundRenderTarget != nullptr && context->boundRenderTarget->getID()==id){
            Application::getContext().getDefaultRenderTarget()->bind();
        }

        glDeleteFramebuffers(1, &renderID);
        glDeleteRenderbuffers(1, &depthAttachmentID);
    }

    void OpenGLRenderTargetImpl::bind() {
        if(context->boundRenderTarget == nullptr || context->boundRenderTarget->getID() != id){
            glBindFramebuffer(GL_FRAMEBUFFER, renderID);
            context->boundRenderTarget = this;
        }
    }

    void OpenGLRenderTargetImpl::unbind() {
        Application::getContext().getDefaultRenderTarget()->bind();
    }

    std::shared_ptr<Texture> OpenGLRenderTargetImpl::getColorAttachment() {
        return colorAttachment;
    }


    OpenGLRenderTargetImplDefaultOuput::OpenGLRenderTargetImplDefaultOuput(GraphicsContext *context) : RenderTargetImpl(
            context) {}

    OpenGLRenderTargetImplDefaultOuput::~OpenGLRenderTargetImplDefaultOuput() = default;

    void OpenGLRenderTargetImplDefaultOuput::bind() {
        if(context->boundRenderTarget != nullptr){
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            context->boundRenderTarget = nullptr;
        }
    }

    void OpenGLRenderTargetImplDefaultOuput::unbind() {
        bind();
    }

    std::shared_ptr<Texture> OpenGLRenderTargetImplDefaultOuput::getColorAttachment() {
        return std::shared_ptr<Texture>();
    }
}