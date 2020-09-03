/*
 * OpenGLContext.h
 *
 *  Created on: 24.07.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/GraphicsContext.h"

struct GLFWwindow;

namespace Zoe {

/**
 * Implementation of GraphicsContext\n
 * OpenGl should be used if no better implementation is available
 */
class DLL_PUBLIC OpenGLContext : public GraphicsContext {
public:
    /**
     * Creates new OpenGLContext for a specific window.
     * @param window the window that provides the OpenGLContext
     */
    OpenGLContext(GLFWwindow *window);

    /**
     * Initializes this object
     */
    void init() override;

    /**
     * Swaps the buffers of the saved window.
     */
    void swapBuffers() override;

    /**
     * Creates a new Index buffer. The returned object is static if `dynamicBuffer == false` and dynamic if `dynamicBuffer == true`.
     * @param dynamicBuffer whether the buffer is dynamic or not
     * @returns the new IndexBuffer
     * @see OpenGLIndexBufferImpl
     */
    std::shared_ptr<IndexBuffer> getIndexBufferV(bool dynamicBuffer) override;

    /**
     * Creates a new Render Object.
     * @returns the new Render Object
     * @see OpenGLRenderImpl
     */
    std::shared_ptr<Render> getRenderV() override;

    /**
     * Creates a new Shader Object.
     * @param file the File used for the creation of the Shader
     * @returns the new Shader
     * @see OpenGLShaderImpl
     */
    std::shared_ptr<Shader> getShaderV(const File &file) override;

    /**
     * Creates a new Vertex Array.
     * @returns the new VertexArray
     * @see OpenGLVertexArrayImpl
     */
    std::shared_ptr<VertexArray> getVertexArrayV() override;

    /**
     * Creates a new Vertex buffer. The returned object is static if `dynamicBuffer == false` and dynamic if `dynamicBuffer == true`.
     * @param dynamicBuffer whether the buffer is dynamic or not
     * @returns the new VertexBuffer
     * @see OpenGLVertexBufferImpl
     */
    std::shared_ptr<VertexBuffer> getVertexBufferV(bool dynamicBuffer) override;

    /**
     * Creates a new VertexBufferLayer.
     * @returns the new VertexBufferLayout
     * @see OpenGLVertexBufferLayoutImpl
     */
    std::shared_ptr<VertexBufferLayout> getVertexBufferLayoutV() override;

    /**
     * Creates an empty texture object with specified width, height and channel amount.
     * @param width width of the texture object
     * @param height height of the texture object
     * @param channels channel amount of the texture object. Values can be 1 or 4
     * @returns the new Texture
     * @see OpenGLTextureImpl
     */
    std::shared_ptr<Texture> getTextureV(unsigned int width, unsigned int height, unsigned int channels) override;

    /**
     * Creates a texture object and reads the specified .webp file.
     * @param file the .webp file
     * @returns the new Texture
     * @see OpenGLTextureImpl
     */
    std::shared_ptr<Texture> getTextureV(const File &file) override;

    /**
     * Creates a custom render target with specified width and height. A render target has a RGBA texture and a depth buffer.
     * @param width width of the target
     * @param height height of the target
     * @returns the new RenderTarget
     * @see OpenGLRenderTargetImpl
     */
    std::shared_ptr<RenderTarget> getRenderTargetV(unsigned int width, unsigned int height) override;

    /**
     * Returns the default render target
     * @returns the default render target
     * @see OpenGLRenderTargetImpl
     */
    std::shared_ptr<RenderTarget> getDefaultRenderTargetV() override;

private:
    GLFWwindow *windowHandle;

    std::shared_ptr<RenderTarget> defaultRenderTarget;
};

}

