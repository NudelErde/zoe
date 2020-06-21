/*
 * Texture.h
 *
 *  Created on: 16.12.2019
 *      Author: florian
 */

#pragma once

#include <cinttypes>

#include "../../Core.h"
#include "ImplPointer.h"
#include "../../File.h"
#include "../GraphicsContext.h"

namespace Zoe{

class GraphicsContext;

class TextureImpl{
public:
	TextureImpl(GraphicsContext* context): context(context), id(GraphicsContext::generateID()){}
	virtual ~TextureImpl(){}

	virtual void bind(unsigned int slot) = 0;
	virtual void unbind(unsigned int slot) = 0;

	virtual void setData(uint8_t* data,unsigned int size) = 0;
	virtual void setData(const File& file) = 0;

	virtual unsigned int getWidth() = 0;
	virtual unsigned int getHeight() = 0;

    inline const int& getID() const{ return id;}

protected:
	GraphicsContext* context;

	int id;
};

class DLL_PUBLIC Texture{
public:
	Texture(TextureImpl* impl):impl(impl){};
	~Texture(){}

	inline void bind(unsigned int slot){impl->bind(slot);}
	inline void unbind(unsigned int slot){impl->unbind(slot);}

	inline void setData(uint8_t* data, unsigned int size){impl->setData(data,size);}
	inline void setData(const File& file){impl->setData(file);}

	inline unsigned int getWidth() {return impl->getWidth();}
	inline unsigned int getHeiht() {return impl->getHeight();}

    inline TextureImpl* getImpl() {return &impl;}

    inline const int& getID() const { return impl->getID();}

private:
	ImplPointer<TextureImpl> impl;
};

}
