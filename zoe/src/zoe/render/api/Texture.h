/*
 * Texture.h
 *
 *  Created on: 16.12.2019
 *      Author: florian
 */

#pragma once

#include <cinttypes>

#include "../../core/Core.h"
#include "ImplPointer.h"
#include "../../core/File.h"
#include "../GraphicsContext.h"

namespace Zoe {

class GraphicsContext;

/**
 * This class should not be used directly.
 * It is used to provide a level of abstraction for Texture.
 */
class TextureImpl {
public:
    /**
     * Creates a TextureImpl with the specified context and generates an ID.
     * @param context the specified context
     */
    TextureImpl(GraphicsContext* context) : context(context), id(GraphicsContext::generateID()) {}

    /**
     * Destructs this object.
     */
    virtual ~TextureImpl() {}

    /**
     * Binds this Texture to the specified slot. See implementation for details.
     * @param slot the specified slot
     */
    virtual void bind(unsigned int slot) = 0;

    /**
     * Unbinds this Texture from the specified slot. See implementation for details.
     * @param slot the specified slot
     */
    virtual void unbind(unsigned int slot) = 0;

    /**
     * Stores the data with the specified size. Size must equal `width * height * channels`. The data must have one byte per channel and pixel.
     * If channels is 4, data must be in RGBA order from to left to right.
     * See implementation for details.
     * @param data the data of the texture
     * @param size the size of data
     */
    virtual void setData(uint8_t* data, unsigned int size) = 0;

    /**
     * Loads a texture from the specified file.
     * See implementation for details.
     * @param file the specified file
     */
    virtual void setData(const File& file) = 0;

    /**
     * Returns the width of the texture.
     * See implementation for details.
     * @return the width of the texture
     */
    virtual unsigned int getWidth() = 0;

    /**
     * Returns the height of the texture.
     * See implementation for details.
     * @return the height of the texture
     */
    virtual unsigned int getHeight() = 0;

    /**
     * Returns the ID of this object. The ID is unique.
     * @return the ID
     */
    inline const int& getID() const { return id; }

protected:
    GraphicsContext* context;

    int id;
};

/**
 * A texture contains a two-dimensional array of pixels in 1 to 4 channels in video RAM.
 */
class DLL_PUBLIC Texture {
public:

    /**
     * Creates a VertexArray with the specified implementation.
     * @param impl the specified implementation
     */
    Texture(TextureImpl* impl) : impl(impl) {};

    /**
     * Binds this Texture to the specified slot.
     * @param slot the specified slot
     */
    inline void bind(unsigned int slot) { impl->bind(slot); }

    /**
     * Unbinds this Texture from the specified slot.
     * @param slot the specified slot
     */
    inline void unbind(unsigned int slot) { impl->unbind(slot); }

    /**
     * Stores the data with the specified size. Size must equal `width * height * channels`. The data must have one byte per channel and pixel.
     * If channels is 4, data must be in RGBA order from to left to right.
     * @param data the data of the texture
     * @param size the size of data
     */
    inline void setData(uint8_t* data, unsigned int size) { impl->setData(data, size); }

    /**
     * Loads a texture from the specified file.
     * @param file the specified file
     */
    inline void setData(const File& file) { impl->setData(file); }

    /**
     * Returns the width of the texture.
     * @return the width of the texture
     */
    inline unsigned int getWidth() { return impl->getWidth(); }

    /**
     * Returns the height of the texture.
     * @return the height of the texture
     */
    inline unsigned int getHeight() { return impl->getHeight(); }

    /**
     * Returns the underlying implementation.
     * @return the implementation
     */
    inline TextureImpl* getImpl() { return &impl; }

    /**
     * Returns the ID of this object. The ID is unique for the implementation.
     * @return the ID
     */
    inline const int& getID() const { return impl->getID(); }

private:
    ImplPointer <TextureImpl> impl;
};

}
