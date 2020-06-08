/*
 * OpenGLTextureImpl.cpp
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#include "OpenGLTextureImpl.h"
#include "../../zoe/Console.h"
#include "GL/glew.h"
#include "webp/decode.h"

namespace Zoe {

OpenGLTextureImpl::OpenGLTextureImpl(GraphicsContext* context, const File& file) :
		TextureImpl(context), dataFormat(GL_RGBA),internalFormat(GL_RGBA8), channels(4) {
	size_t size = 0;
	std::unique_ptr<uint8_t[]> data = file.getByteArray(&size);
	WebPBitstreamFeatures features;
	VP8StatusCode status = WebPGetFeatures(data.get(), size, &features);
	if(status != VP8_STATUS_OK){
		error("Could not load texture from file: ", file.getName());
		renderID = 0;
		return;
	}
	this->height = features.height;
	this->width = features.width;

	uint8_t* decoded = WebPDecodeRGBA(data.get(), size, &(features.width), &(features.height));

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &renderID);
	glBindTexture(GL_TEXTURE_2D, renderID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, decoded);
	WebPFree(decoded);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTextureImpl::OpenGLTextureImpl(GraphicsContext* context,
		const unsigned int& width, const unsigned int& height, unsigned int channels) :
		TextureImpl(context), width(width), height(height), channels(channels) {
    switch (channels) {
        case 1:
            dataFormat = GL_RED;
            internalFormat = GL_R8;
            break;
        case 4:
            dataFormat = GL_RGBA;
            internalFormat = GL_RGBA8;
            break;
        default:
            warning("Textures with ", channels, " are not supported!");
            return;
    }
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &renderID);
	glBindTexture(GL_TEXTURE_2D, renderID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLTextureImpl::~OpenGLTextureImpl() {
	glDeleteTextures(1, &renderID);
}

void OpenGLTextureImpl::bind(unsigned int slot) {
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, renderID);
}

void OpenGLTextureImpl::unbind(unsigned int slot) {
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLTextureImpl::setData(uint8_t* data, unsigned int size) {
	if(width*height*channels != size){
		error("Could not load texture. Data size is invalid: ",size," should be ", width*height*channels);
		return;
	}

    bind(0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTextureImpl::setData(const File& file) {
	size_t size = 0;
	std::unique_ptr<uint8_t[]> data = file.getByteArray(&size);
	WebPBitstreamFeatures features;
	VP8StatusCode status = WebPGetFeatures(data.get(), size, &features);
	if(status != VP8_STATUS_OK){
		error("Could not load texture from file: ", file.getName());
		return;
	}

	uint8_t* decoded = WebPDecodeRGBA(data.get(), size, &(features.width), &(features.height));

	bind(0);
	glTexSubImage2D(GL_TEXTURE0, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, decoded);
	WebPFree(decoded);
}

unsigned int OpenGLTextureImpl::getWidth() {
	return width;
}

unsigned int OpenGLTextureImpl::getHeight() {
	return height;
}

}
