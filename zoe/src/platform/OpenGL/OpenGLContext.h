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

	std::shared_ptr<IndexBuffer> getIndexBuffer() override;
	std::shared_ptr<Render> getRender() override;
	std::shared_ptr<Shader> getShader(const File& file) override;
	std::shared_ptr<VertexArray> getVertexArray() override;
	std::shared_ptr<VertexBuffer> getVertexBuffer() override;
	std::shared_ptr<VertexBufferLayout> getVertexBufferLayout() override;
	std::shared_ptr<Texture> getTexture(unsigned int width, unsigned int height) override;
	std::shared_ptr<Texture> getTexture(const File& file) override;
private:
	GLFWwindow* windowHandle;
};

}

