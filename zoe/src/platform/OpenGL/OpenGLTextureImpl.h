/*
 * OpenGLTextureImpl.h
 *
 *  Created on: 17.12.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/render/api/Texture.h"

namespace Zoe {

/**
 * Implementation of Texture.\n
 * OpenGl should be used if no better implementation is available.\n
 * An OpenGLTextureImpl has 4 color channels (RGBA).\n
 */
class OpenGLTextureImpl : public TextureImpl {
public:
    /**
     * Creates an OpenGLTexture and loads the content from the specified file.
     * @param context the GraphicsContext. Must be OpenGLContext
     * @param file the specified file
     */
    OpenGLTextureImpl(GraphicsContext *context, const File &file);

    /**
     * Creates an OpenGLTexture with the specified width, height and channel count.
     * @param context the GraphicsContext. Must be OpenGLContext
     * @param width the width of the texture
     * @param height the height of the texture
     * @param channels the channel count. Must be 1 or 4
     */
    OpenGLTextureImpl(GraphicsContext *context, const unsigned int &width, const unsigned int &height,
                      unsigned int channels);

    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLTextureImpl();

    /**
     * Binds the OpenGLTexture to the specified slot if not already bound.
     * @param slot the specified slot
     */
    void bind(unsigned int slot);

    /**
     * Unbinds the OpenGLTexture on the specified slot
     * @param slot the specified slot
     */
    void unbind(unsigned int slot);

    /**
     * Stores the data with the specified size. Size must equal `width * height * channels`. The data must be one byte per channel.
     * If channels is 4, data must be in RGBA order from to left to right.
     * @param data the data of the texture
     * @param size the size of data
     */
    void setData(uint8_t *data, unsigned int size);

    /**
     * Loads a texture from the specified file.
     * @param file the specified file
     */
    void setData(const File &file);

    /**
     * Returns the width of the texture.
     * @return the width of the texture
     */
    unsigned int getWidth();

    /**
     * Returns the height of the texture.
     * @return the height of the texture
     */
    unsigned int getHeight();

    /**
     * Return native renderID. This should only be used by other OpenGL classes.
     * @return native renderID
     */
    inline unsigned int getHandle() const { return renderID; }

private:
    unsigned int dataFormat, internalFormat;
    unsigned int width, height;
    unsigned int channels;
    unsigned int renderID;
};

}


