/*
 * Render.h
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#pragma once

#include "../../core/Core.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "ImplPointer.h"
#include "../../math/vec.h"

namespace Zoe {

class RenderTarget;

/**
 * This class should not be used directly.
 * It is used to provide a level of abstraction for Render.
 */
class RenderImpl {
public:
    /**
     * Creates A RenderImpl with the specified context, the specified viewport and generates an ID.
     * @param context the specified context.
     * @param x the X coordinate of the lower left corner
     * @param y the Y coordinate of the lower left corner
     * @param width the width of the viewport
     * @param height the height of the viewport
     */
    RenderImpl(GraphicsContext* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
            : settings({0, {0, 0, 0, 1}, x, y, width, height}), context(context), id(GraphicsContext::generateID()) {
    }

    /**
     * Destructs this object.
     */
    virtual ~RenderImpl() {}

    /**
     * Clears the target buffer using the stored clear color.
     * See implementation for details.
     */
    virtual void clear() = 0;

    /**
     * Sets the clear color.
     * See implementation for details.
     * @param r the red component
     * @param g the green component
     * @param b the blue component
     * @param a the alpha component
     */
    virtual void setClearColor(float r, float g, float b, float a) = 0;

    /**
     * Draws the specified VertexArray with the specified Shader.
     * See implementation for details.
     * @param va the VertexArray
     * @param shader the Shader
     */
    virtual void draw(VertexArray& va, Shader& shader) = 0;

    /**
     * Sets the viewport to the specified values.
     * Specify the lower left corner of the viewport rectangle, in pixels.
     * Specify the width and height of the viewport.
     * See implementation for details.
     * @param x the X coordinate of the lower left corner
     * @param y the Y coordinate of the lower left corner
     * @param width the width of the viewport
     * @param height the height of the viewport
     */
    virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

    /**
     * Sets if Alpha should be used while drawing.
     * See implementation for details.
     * @param enable `true` if alpha should be enabled
     */
    virtual void setAlphaEnabled(bool enable) = 0;

    /**
     * Sets the used RenderTarget.
     * See implementation for details.
     * @param renderTarget the RenderTarget
     */
    virtual void setRenderTarget(std::shared_ptr<RenderTarget> renderTarget) = 0;

    /**
     * Pushes the currently selected settings on a stack.
     * See implementation for details.
     */
    virtual void push() = 0;

    /**
     * Pops the settings stack. Restores the previously pushed settings.
     * See implementation for details.
     */
    virtual void pop() = 0;

    /**
     * Returns the clear color as a vec4.
     * @return the clear color
     */
    inline vec4 getClearColor() { return settings.clearColor; }

    /**
     * Returns the X coordinate of the lower left corner.
     * @return the X coordinate of the lower left corner
     */
    inline unsigned int getViewportX() { return settings.x; }

    /**
     * Returns the Y coordinate of the lower left corner.
     * @return the Y coordinate of the lower left corner
     */
    inline unsigned int getViewportY() { return settings.y; }

    /**
     * Returns the width of the viewport.
     * @return the width of the viewport
     */
    inline unsigned int getViewportWidth() { return settings.width; }

    /**
     * Returns the height of the viewport.
     * @return the height of the viewport
     */
    inline unsigned int getViewportHeight() { return settings.height; }

    /**
     * Returns the RenderFlag.
     * @return the RenderFlag
     */
    inline RenderFlag getSettingsFlag() { return settings.flag; }

    /**
     * Returns the ID of this object. The ID is unique.
     * @return the ID
     */
    inline const int& getID() const { return id; }

protected:
    /**
     * The used graphics context.
     */
    GraphicsContext* context;

    /**
     * The save settings.
     */
    RenderSettings settings;

    /**
     * A unique id.
     */
    int id;

    friend class Render;
};

/**
 * A Render Object contains information about the RenderTarget and selected settings.
 */
class DLL_PUBLIC Render {
public:
    /**
     * Creates a Render with the specified implementation.
     * @param impl the specified implementation
     */
    Render(RenderImpl* impl) : impl(impl) {}

    /**
     * Clears the target buffer using the stored clear color.
     */
    inline void clear() { impl->clear(); }

    /**
     * Sets the clear color.
     * @param r the red component
     * @param g the green component
     * @param b the blue component
     * @param a the alpha component
     */
    inline void setClearColor(float r, float g, float b, float a) { impl->setClearColor(r, g, b, a); }

    /**
     * Draws the specified VertexArray with the specified Shader.
     * @param va the VertexArray
     * @param shader the Shader
     */
    inline void draw(VertexArray& va, Shader& shader) { impl->draw(va, shader); }

    /**
     * Sets the viewport to the specified values.
     * Specify the lower left corner of the viewport rectangle, in pixels.
     * Specify the width and height of the viewport.
     * @param x the X coordinate of the lower left corner
     * @param y the Y coordinate of the lower left corner
     * @param width the width of the viewport
     * @param height the height of the viewport
     */
    inline void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
        impl->setViewport(x, y, width, height);
    }

    /**
     * Sets if Alpha should be used while drawing.
     * @param enable `true` if alpha should be enabled
     */
    inline void setAlphaEnabled(bool enable) { impl->setAlphaEnabled(enable); }

    /**
     * Returns the clear color as a vec4.
     * @return the clear color
     */
    inline vec4 getClearColor() { return impl->settings.clearColor; }

    /**
     * Returns the X coordinate of the lower left corner.
     * @return the X coordinate of the lower left corner
     */
    inline unsigned int getViewportX() { return impl->settings.x; }

    /**
     * Returns the Y coordinate of the lower left corner.
     * @return the Y coordinate of the lower left corner
     */
    inline unsigned int getViewportY() { return impl->settings.y; }

    /**
     * Returns the width of the viewport.
     * @return the width of the viewport
     */
    inline unsigned int getViewportWidth() { return impl->settings.width; }

    /**
     * Returns the height of the viewport.
     * @return the height of the viewport
     */
    inline unsigned int getViewportHeight() { return impl->settings.height; }

    /**
     * Returns the RenderFlag.
     * @return the RenderFlag
     */
    inline RenderFlag getSettingsFlag() { return impl->settings.flag; }

    /**
     * Sets the used RenderTarget.
     * @param target the RenderTarget
     */
    inline void setRenderTarget(std::shared_ptr<RenderTarget> target) { impl->setRenderTarget(target); }

    /**
    * Returns the ID of this object. The ID is unique for the implementation.
    * @return the ID
    */
    inline const int& getID() const { return impl->getID(); }

    /**
     * Pushes the currently selected settings on a stack.
     */
    inline void push() { impl->push(); }

    /**
     * Pops the settings stack. Restores the previously pushed settings.
     * See implementation for details.
     */
    inline void pop() { impl->pop(); }
private:
    ImplPointer<RenderImpl> impl;
};

}
