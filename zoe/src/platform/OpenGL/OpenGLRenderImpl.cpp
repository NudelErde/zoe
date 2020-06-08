/*
 * OpenGLRenderImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#include <ObjIdl.h>
#include <freetype/ftglyph.h>
#include "OpenGLRenderImpl.h"

#include "../../zoe/render/GraphicsContext.h"
#include "../../zoe/Application.h"

#include "OpenGLVertexArrayImpl.h"
#include "OpenGLIndexBufferImpl.h"

namespace Zoe {

OpenGLRenderImpl::OpenGLRenderImpl(GraphicsContext* context) :
		RenderImpl(context, 0, 0, 1280, 720) {
	settings.width = Application::get().getWindow().getWidth();
	settings.height = Application::get().getWindow().getHeight();
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
    settings.clearColor = {r,g,b,a};
}

void OpenGLRenderImpl::setViewport(unsigned int x, unsigned int y,
		unsigned int width, unsigned int height) {
	this->settings.x = x;
	this->settings.y = y;
	this->settings.width = width;
	this->settings.height = height;
}

void OpenGLRenderImpl::setAlphaEnabled(bool enabled) {
    settings.flag.alpha = enabled;
}

void OpenGLRenderImpl::loadSettings() {
	Settings& bound = context->boundRenderSettings;

	//Update viewport
    if (bound.x != settings.x || bound.y != settings.y || bound.width != settings.width ||
        bound.height != settings.height) {
        glViewport(settings.x,settings.y,settings.width,settings.height);
    }

    //Update clearColor
    if(bound.clearColor != settings.clearColor){
        glClearColor(settings.clearColor.x, settings.clearColor.y, settings.clearColor.z, settings.clearColor.w);
    }

    //Check alpha enabled
    if(bound.flag.alpha != settings.flag.alpha){
        if(settings.flag.alpha){
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
        } else {
            glDisable(GL_BLEND);
        }
    }
    bound = settings;
}

}
