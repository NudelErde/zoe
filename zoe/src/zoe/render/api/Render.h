/*
 * Render.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../Core.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ImplPointer.h"
#include "../../math/vec.h"

namespace Zoe{

class RenderTarget;

class RenderImpl{

public:
    RenderImpl(GraphicsContext *context, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
            : settings({0, {0, 0, 0, 1}, x, y, width, height}), context(context), id(GraphicsContext::generateID()) {
    }
	virtual ~RenderImpl(){}

	virtual void clear() = 0;
	virtual void setClearColor(float r,float g,float b,float a) = 0;
	virtual void draw(VertexArray& va,Shader& shader) = 0;

	virtual void setViewport(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom) = 0;
	virtual void setAlphaEnabled(bool enable) = 0;

	virtual void setRenderTarget(std::shared_ptr<RenderTarget> renderTarget) = 0;

	inline vec4 getClearColor(){return settings.clearColor;}
	inline unsigned int getViewportX(){return settings.x;}
	inline unsigned int getViewportY(){return settings.y;}
	inline unsigned int getViewportWidth(){return settings.width;}
	inline unsigned int getViewportHeight(){return settings.height;}
	inline RenderFlag getSettingsFlag(){return settings.flag;}

    inline const int& getID() const{ return id;}

protected:
	GraphicsContext* context;
    RenderSettings settings;

    int id;

	friend class Render;
};

class DLL_PUBLIC Render{
public:
	Render(RenderImpl* impl):impl(impl){}
	~Render(){}
	inline void clear(){impl->clear();}
	inline void setClearColor(float r,float g, float b, float a){impl->setClearColor(r,g,b,a);}
	inline void draw(VertexArray& va,Shader& shader){impl->draw(va,shader);}

	inline void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height){impl->setViewport(x, y, width, height);}
	inline void setAlphaEnabled(bool enable){impl->setAlphaEnabled(enable);}

	inline vec4 getClearColor(){return impl->settings.clearColor;}
	inline unsigned int getViewportX(){return impl->settings.x;}
	inline unsigned int getViewportY(){return impl->settings.y;}
	inline unsigned int getViewportWidth(){return impl->settings.width;}
	inline unsigned int getViewportHeight(){return impl->settings.height;}
	inline RenderFlag getSettingsFlag(){return impl->settings.flag;}

	inline void setRenderTarget(std::shared_ptr<RenderTarget> target){impl->setRenderTarget(target);}

    inline const int& getID() const { return impl->getID();}
private:
	ImplPointer<RenderImpl> impl;
};

}
