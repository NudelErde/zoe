/*
 * OpenGLRenderImpl.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/Render.h"
#include "../../zoe/math/vec.h"
#include <GL/glew.h>

namespace Zoe{

class GraphicsContext;

class OpenGLRenderImpl: public RenderImpl{
public:
	OpenGLRenderImpl(GraphicsContext* context);
	~OpenGLRenderImpl();

	void draw(VertexArray& va,Shader& shader);
	void setClearColor(float r,float g,float b,float a);
	void clear();

	void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	void setAlphaEnabled(bool enabled) override;

	void setRenderTarget(std::shared_ptr<RenderTarget> renderTarget) override;

	void loadSettings();
private:
    std::weak_ptr<RenderTarget> renderTarget;
};

}
