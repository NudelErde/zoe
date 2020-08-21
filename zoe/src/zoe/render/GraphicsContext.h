/*
 * GraphicsContext.h
 *
 *  Created on: 24.07.2019
 *      Author: florian
 */

#pragma once

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

namespace Zoe{

class DLL_PUBLIC GraphicsContext{
public:
	GraphicsContext()= default;
	virtual ~GraphicsContext()= default;
	virtual void init() = 0;
	virtual void swapBuffers() = 0;

    inline std::shared_ptr<IndexBuffer> getIndexBuffer(bool dynamicBuffer = false) {return getIndexBufferV(dynamicBuffer);};
    inline std::shared_ptr<Render> getRender() {return getRenderV();}
    inline std::shared_ptr<Shader> getShader(const File& file) {return getShaderV(file);}
    inline std::shared_ptr<VertexArray> getVertexArray() {return getVertexArrayV();}
    inline std::shared_ptr<VertexBuffer> getVertexBuffer(bool dynamicBuffer = false) {return getVertexBufferV(dynamicBuffer);}
    inline std::shared_ptr<VertexBufferLayout> getVertexBufferLayout() {return getVertexBufferLayoutV();}
    inline std::shared_ptr<Texture> getTexture(unsigned int width, unsigned int height, unsigned int channels)
                {return getTextureV(width, height, channels);}
    inline std::shared_ptr<Texture> getTexture(const File& file) {return getTextureV(file);}
    inline std::shared_ptr<RenderTarget> getRenderTarget(unsigned int width, unsigned int height) {return getRenderTargetV(width, height);}
    inline std::shared_ptr<RenderTarget> getDefaultRenderTarget() {return getDefaultRenderTargetV();}

public:
    IndexBufferImpl* boundIndexBuffer = nullptr;
    VertexBufferImpl* boundVertexBuffer = nullptr;
    ShaderImpl* boundShader = nullptr;
    RenderTargetImpl* boundRenderTarget = nullptr;
    VertexArrayImpl* boundVertexArray = nullptr;
    RenderSettings boundRenderSettings{};

public:
    static const int& generateID();

protected:
    virtual std::shared_ptr<IndexBuffer> getIndexBufferV(bool dynamicBuffer) = 0;
    virtual std::shared_ptr<Render> getRenderV() = 0;
    virtual std::shared_ptr<Shader> getShaderV(const File& file) = 0;
    virtual std::shared_ptr<VertexArray> getVertexArrayV() = 0;
    virtual std::shared_ptr<VertexBuffer> getVertexBufferV(bool dynamicBuffer) = 0;
    virtual std::shared_ptr<VertexBufferLayout> getVertexBufferLayoutV() = 0;
    virtual std::shared_ptr<Texture> getTextureV(unsigned int width, unsigned int height, unsigned int channels) = 0;
    virtual std::shared_ptr<Texture> getTextureV(const File& file) = 0;
    virtual std::shared_ptr<RenderTarget> getRenderTargetV(unsigned int width, unsigned int height) = 0;
    virtual std::shared_ptr<RenderTarget> getDefaultRenderTargetV() = 0;

};

}


