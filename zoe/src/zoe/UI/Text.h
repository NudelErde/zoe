//
// Created by Florian on 06.06.2020.
//

#pragma once

#include "../Core.h"
#include "CommonUI.h"
#include "../File.h"
#include "../render/api/Texture.h"
#include "Font.h"
#include "../render/GraphicsContext.h"

namespace Zoe {

    struct RenderChar;

    class Text : public Component {
    public:
        Text(const Point& begin, const Font& font, std::string string, const Color& color);
        ~Text() override;

        void draw(std::shared_ptr<Render> renderer) override;

        void setText(std::string str);
        void setFont(const Font& font);
        const std::string& getText();
        const Font& getFont();
    private:
        Font font;
        std::shared_ptr<Texture> bitmap;
        std::string string;
        Point begin;
        Color color;

        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<IndexBuffer> indexBuffer;
        std::shared_ptr<VertexArray> vertexArray;

        unsigned int* indexData;
        RenderChar* vertexData;
    };

}
