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

class GraphicsContext;

enum class RenderFlag: unsigned int{
	ALPHA_FLAG = 0b1
};

class RenderImpl{
public:
	RenderImpl(GraphicsContext* context,unsigned int x, unsigned int y, unsigned int width, unsigned int height):  settingsFlag(0),clearColor({0,0,0,1}),x(x),y(y),width(width), height(height){
		this->context = context;
	}
	virtual ~RenderImpl(){}

	virtual void clear() = 0;
	virtual void setClearColor(float r,float g,float b,float a) = 0;
	virtual void draw(VertexArray& va,Shader& shader) = 0;

	virtual void setViewport(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom) = 0;
	virtual void setAlphaEnabled(bool enable) = 0;

	inline vec4 getClearColor(){return clearColor;}
	inline unsigned int getViewportX(){return x;}
	inline unsigned int getViewportY(){return y;}
	inline unsigned int getViewportWidth(){return width;}
	inline unsigned int getViewportHeight(){return height;}
	inline unsigned int getSettingsFlag(){return settingsFlag;}

protected:
	GraphicsContext* context;
	unsigned int settingsFlag;
	vec4 clearColor;
	unsigned int x,y,width,height;

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

	inline vec4 getClearColor(){return impl->clearColor;}
	inline unsigned int getViewportX(){return impl->x;}
	inline unsigned int getViewportY(){return impl->y;}
	inline unsigned int getViewportWidth(){return impl->width;}
	inline unsigned int getViewportHeight(){return impl->height;}
	inline unsigned int getSettingsFlag(){return impl->settingsFlag;}
private:
	ImplPointer<RenderImpl> impl;
};

}
