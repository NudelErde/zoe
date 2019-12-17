/*
 * OpenGLTextureImpl.h
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/Texture.h"

namespace Zoe{

class OpenGLTextureImpl: public TextureImpl{
public:
	OpenGLTextureImpl(GraphicsContext* context, const File& file);
	OpenGLTextureImpl(GraphicsContext* context, const unsigned int& width, const unsigned int& height);
	~OpenGLTextureImpl();

	void bind(unsigned int slot);
	void unbind(unsigned int slot);

	void setData(uint8_t* data, unsigned int size);
	void setData(const File& file);

	unsigned int getWidth();
	unsigned int getHeight();
private:
	unsigned int dataFormat,internalFormat;
	unsigned int width,height;
	unsigned int renderID;
};

}


