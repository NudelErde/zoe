/*
 * OpenGLContext.cpp
 *
 *  Created on: 24.07.2019
 *      Author: florian
 */

#include "OpenGLContext.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Console.h"
#include "OpenGLIndexBufferImpl.h"
#include "OpenGLRenderImpl.h"
#include "OpenGLShaderImpl.h"
#include "OpenGLVertexArrayImpl.h"
#include "OpenGLVertexBufferImpl.h"
#include "OpenGLVertexBufferLayoutImpl.h"

namespace Zoe {

OpenGLContext::OpenGLContext(GLFWwindow* window): GraphicsContext(),
		windowHandle(window) {

}

void OpenGLContext::init() {
	glfwMakeContextCurrent(windowHandle);
	info("OpenGLContext:");
	info("  Vendor: \t", glGetString(GL_VENDOR));
	info("  Renderer:\t", glGetString(GL_RENDERER));
	info("  Version:\t", glGetString(GL_VERSION));
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glClearDepth(1.0f);
}

void OpenGLContext::swapBuffers() {
	glfwSwapBuffers(windowHandle);
}

IndexBuffer* OpenGLContext::getIndexBuffer(){
	return new IndexBuffer(new OpenGLIndexBufferImpl(this));
}

Render* OpenGLContext::getRender(){
	return new Render(new OpenGLRenderImpl(this));
}

Shader* OpenGLContext::getShader(const File& file){
	return new Shader(new OpenGLShaderImpl(file,this));
}

VertexArray* OpenGLContext::getVertexArray(){
	return new VertexArray(new OpenGLVertexArrayImpl(this));
}

VertexBuffer* OpenGLContext::getVertexBuffer(){
	return new VertexBuffer(new OpenGLVertexBufferImpl(this));
}

VertexBufferLayout* OpenGLContext::getVertexBufferLayout(){
	return new VertexBufferLayout(new OpenGLVertexBufferLayoutImpl());
}

}

