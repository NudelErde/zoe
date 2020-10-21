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
#include <stack>

namespace Zoe {

class GraphicsContext;

/**
 * Implementation of Render.\n
 * OpenGl should be used if no better implementation is available.\n
 * A Render Object contains information about the RenderTarget and selected settings.
 */
class OpenGLRenderImpl : public RenderImpl {
public:
    /**
     * Creates an OpenGLRender Object.
     * @param context the GraphicsContext. Must be OpenGLContext
     */
    OpenGLRenderImpl(GraphicsContext *context);

    /**
     * Releases all used OpenGL resources for this object.
     */
    ~OpenGLRenderImpl();

    /**
     * Draws the specified VertexArray with the specified Shader.
     * @param va the VertexArray
     * @param shader the Shader
     */
    void draw(VertexArray &va, Shader &shader);

    /**
     * Sets the clear color.
     * @param r the red component
     * @param g the green component
     * @param b the blue component
     * @param a the alpha component
     * @see http://docs.gl/gl3/glClearColor
     */
    void setClearColor(float r, float g, float b, float a);

    /**
     * Clears the color and depth buffer.
     * @see http://docs.gl/gl3/glClear
     */
    void clear();

    /**
     * Sets the viewport to the specified values.
     * Specify the lower left corner of the viewport rectangle, in pixels.
     * Specify the width and height of the viewport.
     * @param x the X coordinate of the lower left corner
     * @param y the Y coordinate of the lower left corner
     * @param width the width of the viewport
     * @param height the height of the viewport
     * @see http://docs.gl/gl3/glViewport
     */
    void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    /**
     * Sets if Alpha should be used while drawing.
     * @param enabled `true` if alpha should be enabled
     */
    void setAlphaEnabled(bool enabled) override;

    /**
     * Pushes the currently selected settings on a stack.
     */
    void push() override;

    /**
     * Pops the settings stack. Restores the previously pushed settings.
     */
    void pop() override;

    /**
     * Sets the used RenderTarget.
     * @param renderTarget the RenderTarget
     */
    void setRenderTarget(std::shared_ptr<RenderTarget> renderTarget) override;

    /**
     * Ensures that the settings are loaded.
     */
    void loadSettings();

private:

    struct StackElement {
        RenderSettings settings;
        std::weak_ptr<RenderTarget> renderTarget;
    };

    std::weak_ptr<RenderTarget> renderTarget;
    std::stack<StackElement> stack;
};

}
