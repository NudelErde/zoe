/*
 * OpenGLContext.h
 *
 *  Created on: 24.07.2019
 *      Author: florian
 */

#pragma once

#include "render/GraphicsContext.h"

struct GLFWwindow;

namespace Zoe {

class DLL_PUBLIC OpenGLContext: public GraphicsContext {
public:
	OpenGLContext(GLFWwindow* window);

	void init() override;
	void swapBuffers() override;

	IndexBuffer* getIndexBuffer() override;
	Render* getRender() override;
	Shader* getShader(const File& file) override;
	VertexArray* getVertexArray() override;
	VertexBuffer* getVertexBuffer() override;
	VertexBufferLayout* getVertexBufferLayout() override;
private:
	GLFWwindow* windowHandle;
};

}

