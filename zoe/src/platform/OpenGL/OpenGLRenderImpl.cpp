/*
 * OpenGLRenderImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#include "OpenGLRenderImpl.h"

#include "../../zoe/render/GraphicsContext.h"
#include "../../zoe/Application.h"

#include "OpenGLVertexArrayImpl.h"
#include "OpenGLIndexBufferImpl.h"

namespace Zoe {

OpenGLRenderImpl::OpenGLRenderImpl(GraphicsContext* context) :
		RenderImpl(context, 0, 0, 1280, 720) {
	width = Application::get().getWindow().getWidth();
	height = Application::get().getWindow().getHeight();
}

OpenGLRenderImpl::~OpenGLRenderImpl() {

}

void OpenGLRenderImpl::draw(VertexArray& va, Shader& shader) {
	loadSettings();
	va.bind();
	shader.bind();
	glDrawElements(GL_TRIANGLES, va.getImpl()->getIndexBuffer().getCount(), GL_UNSIGNED_INT, nullptr);
}

void OpenGLRenderImpl::clear() {
	loadSettings();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderImpl::setClearColor(float r, float g, float b, float a) {
	clearColor = {r,g,b,a};
}

void OpenGLRenderImpl::setViewport(unsigned int x, unsigned int y,
		unsigned int width, unsigned int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

void OpenGLRenderImpl::setAlphaEnabled(bool enabled) {
	settingsFlag ^= (-(unsigned long) enabled ^ settingsFlag)
			& ((unsigned int) RenderFlag::ALPHA_FLAG);
}

void OpenGLRenderImpl::loadSettings() {
	RenderImpl* bound = this->context->boundRender;
	if (bound != nullptr && bound->getClearColor() != this->clearColor) {
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	}
	if (bound != nullptr
			&& !(bound->getViewportX() == x && bound->getViewportY() == y
					&& bound->getViewportWidth() == width
					&& bound->getViewportHeight() == height)) {
		glViewport(x, y, width, height);
	}
	if (bound != nullptr
			&& ((bound->getSettingsFlag() & settingsFlag)
					& (unsigned int) RenderFlag::ALPHA_FLAG)) {
		if (settingsFlag & (unsigned int) RenderFlag::ALPHA_FLAG) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBlendEquation(GL_FUNC_ADD);
		} else {
			glDisable(GL_BLEND);
		}
	}
	this->context->boundRender = this;
}

}
