//
// Created by Florian on 18.07.2020.
//

#pragma once

#include "../../core/Core.h"
#include "../../render/api/Render.h"
#include "../../render/Font.h"
#include "../Camera.h"

namespace Zoe {

class DLL_PUBLIC UITool {
public:
    explicit UITool(const Camera& camera);

    void setColor(const vec4& color);
    void setFont(const Font& font);

    const vec4& getColor();
    const Font& getFont();

    void drawImage(const std::shared_ptr<Texture>& image,const vec2&& pos,const vec2& size);
    void drawRectangle(const vec2& pos, const vec2& size);
    void drawOval(const vec2& pos, const vec2& size);
    void drawText(const std::string&, const vec2& pos);

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