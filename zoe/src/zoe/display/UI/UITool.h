//
// Created by Florian on 18.07.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../../render/api/Render.h"
#include "../../render/Font.h"
#include "../Camera.h"

namespace Zoe {

/**
 * UITool provides useful methods for drawing basic geometry
 */
class DLL_PUBLIC UITool {
public:
    /**
     * Creates UITool with specified target camera
     * @param camera the target camera
     */
    explicit UITool(const Camera& camera);

    /**
     * Sets the color for subsequent drawing method calls
     * @param color the new color
     */
    void setColor(const vec4& color);
    /**
     * Sets the font for subsequent drawText method calls
     * @param font the new font
     */
    void setFont(const Font& font);

    /**
     * Returns currently used color
     * @returns color
     */
    const vec4& getColor();
    /**
     * Returns currently used font
     * @returns font
     */
    const Font& getFont();

    /**
     * Draw the texture at the specified location and size.
     * @param image the texture
     * @param pos the upper left corner of the result
     * @param size the size of the resulting image
     */
    void drawImage(const std::shared_ptr<Texture>& image,const vec2& pos,const vec2& size);
    /**
     * Draw a rectangle at the specified location and size.
     * @param pos the upper left corner of the result
     * @param size the size of the resulting rectangle
     * @see UITool::setColor
     */
    void drawRectangle(const vec2& pos, const vec2& size);
    /**
     * Draw an oval at the specified location and size.
     * @param pos the upper left corner of a rectangle surrounding the oval
     * @param size the size of the resulting oval
     * @see UITool::setColor
     */
    void drawOval(const vec2& pos, const vec2& size);
    /**
     * Draw the specified text in the specified location. The text can contain UTF-8 symbols.
     * @param text the specified text
     * @param pos the position of the left end of the baseline
     */
    void drawText(const std::string& text, const vec2& pos);

    /**
     * Initializes shared UITool data. Must be called before UITool can be used. Application should call this method.
     */
    static void init();

private:
    std::shared_ptr<Render> render;
    vec4 color;
    Font font;
    mat4x4 projectionView;

    std::shared_ptr<VertexBuffer> textVertexBuffer;
    std::shared_ptr<IndexBuffer> textIndexBuffer;
    std::shared_ptr<VertexArray> textVertexArray;
    std::shared_ptr<Texture> textBitmap;
};

}