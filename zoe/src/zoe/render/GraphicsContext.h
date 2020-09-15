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

class DLL_PUBLIC GraphicsContext {
public:
    GraphicsContext() = default;

    virtual ~GraphicsContext() = default;

    virtual void init() = 0;

    virtual void swapBuffers() = 0;

public:
    IndexBufferImpl *boundIndexBuffer = nullptr;
    VertexBufferImpl *boundVertexBuffer = nullptr;
    ShaderImpl *boundShader = nullptr;
    RenderTargetImpl *boundRenderTarget = nullptr;
    VertexArrayImpl *boundVertexArray = nullptr;
    RenderSettings boundRenderSettings{};

public:
    static const int &generateID();

public:
    inline std::shared_ptr<IndexBuffer> getIndexBuffer() { return getIndexBuffer(false); }
    inline std::shared_ptr<VertexBuffer> getVertexBuffer() { return getVertexBuffer(false); }
    inline std::shared_ptr<Shader> getShader(const File& file) { return getShader(file, {}); }

    virtual std::shared_ptr<IndexBuffer> getIndexBuffer(bool dynamicBuffer) = 0;
    virtual std::shared_ptr<Render> getRender() = 0;
    virtual std::shared_ptr<Shader> getShader(const File &file, const std::set<std::string> &options) = 0;
    virtual std::shared_ptr<VertexArray> getVertexArray() = 0;
    virtual std::shared_ptr<VertexBuffer> getVertexBuffer(bool dynamicBuffer) = 0;
    virtual std::shared_ptr<VertexBufferLayout> getVertexBufferLayout() = 0;
    virtual std::shared_ptr<Texture> getTexture(unsigned int width, unsigned int height, unsigned int channels) = 0;
    virtual std::shared_ptr<Texture> getTexture(const File &file) = 0;
    virtual std::shared_ptr<RenderTarget> getRenderTarget(unsigned int width, unsigned int height) = 0;
    virtual std::shared_ptr<RenderTarget> getDefaultRenderTarget() = 0;

};

}


