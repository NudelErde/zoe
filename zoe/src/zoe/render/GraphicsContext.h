/*
 * GraphicsContext.h
 *
 *  Created on: 24.07.2019
 *      Author: florian
 */

#pragma once

#include <set>
#include "../core/Core.h"
#include "../core/File.h"
#include "api/RenderFlags.h"

namespace Zoe {
class IndexBuffer;

class IndexBufferImpl;

class Shader;

class ShaderImpl;

class Texture;

class VertexArray;

class VertexArrayImpl;

class VertexBuffer;

class VertexBufferImpl;

class VertexBufferLayout;

class RenderTarget;

class RenderTargetImpl;

class Render;
} /* namespace Zoe */

namespace Zoe {

/**
 * The Graphics context is the main class of the rendering api. It is used to create the rendering api objects.
 */
class DLL_PUBLIC GraphicsContext {
public:
    /**
     * Creates an empty Graphics context.
     */
    GraphicsContext() = default;

    /**
     * Destructs the Graphics context.
     */
    virtual ~GraphicsContext() = default;

    /**
     * Initializes the rendering api.
     */
    virtual void init() = 0;

    /**
     * Swaps the frame buffers.
     */
    virtual void swapBuffers() = 0;

public:
    /**
     * The currently bound index buffer.
     */
    IndexBufferImpl *boundIndexBuffer = nullptr;

    /**
     * The currently bound vertex buffer.
     */
    VertexBufferImpl *boundVertexBuffer = nullptr;

    /**
     * The currently bound shader.
     */
    ShaderImpl *boundShader = nullptr;

    /**
     * The currently bound render target.
     */
    RenderTargetImpl *boundRenderTarget = nullptr;

    /**
     * The currently bound vertex array.
     */
    VertexArrayImpl *boundVertexArray = nullptr;

    /**
     * The currently active render settings.
     */
    RenderSettings boundRenderSettings{};

public:
    /**
     * Generates a new unique id.
     * @return a new unique id.
     */
    static const int &generateID();

public:

    /**
     * Creates a new IndexBuffer. The returned object is static.
     * @return the new VertexBuffer
     */
    inline std::shared_ptr<IndexBuffer> getIndexBuffer() { return getIndexBuffer(false); }

    /**
     * Creates a new VertexBuffer. The returned object is static.
     * @return the new VertexBuffer
     */
    inline std::shared_ptr<VertexBuffer> getVertexBuffer() { return getVertexBuffer(false); }

    /**
     * Creates a new Shader Object.
     * @param file the File used for the creation of the Shader
     * @returns the new Shader
     */
    inline std::shared_ptr<Shader> getShader(const File& file) { return getShader(file, {}); }

    /**
     * Creates a new Index buffer. The returned object is static if `dynamicBuffer == false` and dynamic if `dynamicBuffer == true`.
     * @param dynamicBuffer whether the buffer is dynamic or not
     * @returns the new IndexBuffer
     */
    virtual std::shared_ptr<IndexBuffer> getIndexBuffer(bool dynamicBuffer) = 0;

    /**
     * Creates a new Render Object.
     * @returns the new Render Object
     */
    virtual std::shared_ptr<Render> getRender() = 0;

    /**
     * Creates a new Shader Object.
     * @param file the File used for the creation of the Shader
     * @param options the options used for the creation of the Shader
     * @returns the new Shader
     */
    virtual std::shared_ptr<Shader> getShader(const File &file, const std::set<std::string> &options) = 0;

    /**
     * Creates a new VertexArray.
     * @returns the new VertexArray
     */
    virtual std::shared_ptr<VertexArray> getVertexArray() = 0;

    /**
     * Creates a new VertexBuffer. The returned object is static if `dynamicBuffer == false` and dynamic if `dynamicBuffer == true`.
     * @param dynamicBuffer whether the buffer is dynamic or not
     * @returns the new VertexBuffer
     */
    virtual std::shared_ptr<VertexBuffer> getVertexBuffer(bool dynamicBuffer) = 0;

    /**
     * Creates a new VertexBufferLayer.
     * @returns the new VertexBufferLayout
     */
    virtual std::shared_ptr<VertexBufferLayout> getVertexBufferLayout() = 0;

    /**
     * Creates an empty texture object with specified width, height and channel amount.
     * @param width width of the texture object
     * @param height height of the texture object
     * @param channels channel amount of the texture object
     * @returns the new Texture
     */
    virtual std::shared_ptr<Texture> getTexture(unsigned int width, unsigned int height, unsigned int channels) = 0;

    /**
     * Creates a texture object and reads the specified .webp file.
     * @param file the .webp file
     * @returns the new Texture
     */
    virtual std::shared_ptr<Texture> getTexture(const File &file) = 0;

    /**
     * Creates a custom render target with specified width and height. A render target has a RGBA texture and a depth buffer.
     * @param width width of the target
     * @param height height of the target
     * @returns the new RenderTarget
     */
    virtual std::shared_ptr<RenderTarget> getRenderTarget(unsigned int width, unsigned int height) = 0;

    /**
     * Returns the default render target.
     * @returns the default render target
     */
    virtual std::shared_ptr<RenderTarget> getDefaultRenderTarget() = 0;

};

}


