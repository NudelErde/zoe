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

class DLL_PUBLIC OpenGLContext: public GraphicsContext {
public:
	OpenGLContext(GLFWwindow* window);

	void init() override;
	void swapBuffers() override;

	std::shared_ptr<IndexBuffer> getIndexBufferV(bool dynamicBuffer) override;
	std::shared_ptr<Render> getRenderV() override;
	std::shared_ptr<Shader> getShaderV(const File& file) override;
	std::shared_ptr<VertexArray> getVertexArrayV() override;
	std::shared_ptr<VertexBuffer> getVertexBufferV(bool dynamicBuffer) override;
	std::shared_ptr<VertexBufferLayout> getVertexBufferLayoutV() override;
	std::shared_ptr<Texture> getTextureV(unsigned int width, unsigned int height, unsigned int channels) override;
	std::shared_ptr<Texture> getTextureV(const File& file) override;
    std::shared_ptr<RenderTarget> getRenderTargetV(unsigned int width, unsigned int height) override;
    std::shared_ptr<RenderTarget> getDefaultRenderTargetV() override;
private:
	GLFWwindow* windowHandle;

	std::shared_ptr<RenderTarget> defaultRenderTarget;
};

}

