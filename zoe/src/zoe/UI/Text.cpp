//
// Created by Florian on 06.06.2020.
//

#include "Text.h"

#include <utility>
#include "../Application.h"
#include "../render/api/Shader.h"
#include "../render/api/IndexBuffer.h"
#include "../render/api/Render.h"
#include "../render/api/Texture.h"
#include "../render/api/VertexArray.h"
#include "../render/api/VertexBuffer.h"
#include "../render/api/VertexBufferLayout.h"
#include "../Console.h"

namespace Zoe {

    struct textData {
        std::shared_ptr<Shader> shader;
        std::shared_ptr<File> file;
    };

    static textData data;

    static bool hasTextInit = false;
    static const char *textShaderSrc =
            R"(#shader vertex
#version 130

layout(location=0) in vec2 position;
layout(location=1) in vec2 texPosition;

out vec2 v_texPosition;

uniform mat4 Projection;

void main(){
	gl_Position = Projection * vec4(position,0,1);
	v_texPosition = texPosition;
}

#shader fragment
#version 130

in vec2 v_texPosition;

out vec4 color;

uniform vec4 u_textColor;
uniform sampler2D tex;

void main(){
    //;
	color = u_textColor*texture2D(tex,v_texPosition).r;
}
)";

    struct CharVertex {
        vec2 position;
        vec2 texPosition;
    };

    struct RenderChar {
        CharVertex bottomLeft;
        CharVertex bottomRight;
        CharVertex topRight;
        CharVertex topLeft;
    };

    Text::Text(const Point &begin, const Font &font, std::string str, const Color &color) : font(font),
                                                                                            string(std::move(str)),
                                                                                            begin(begin), color(color) {
        if (!hasTextInit) {
            hasTextInit = true;
            registerVirtualFile("text.shader", textShaderSrc);
            data.file = std::make_shared<File>("text.shader");
            data.shader = Application::getContext().getShader(*data.file);
            data.shader->setUniform4m("Projection", Zoe::translate3D(0, 0, 0)
                                                    * Zoe::scale3D(1 / 800.0f, -1 / 450.0f, 1) *
                                                    Zoe::translate3D(-800, -450, 0));
        }
        vertexBuffer = Application::getContext().getVertexBuffer(true);
        indexBuffer = Application::getContext().getIndexBuffer(true);
        vertexArray = Application::getContext().getVertexArray();
        vertexData = new RenderChar[64];
        memset(vertexData, 0, sizeof(RenderChar) * 64);
        indexData = new unsigned int[6 * 64];
        memset(indexData, 0, sizeof(unsigned int) * 6 * 64);

        vertexBuffer->setData(vertexData, sizeof(RenderChar) * 64);
        indexBuffer->setData(indexData, 6 * 64);
        std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
        layout->push_float(2);
        layout->push_float(2);
        vertexArray->set(*vertexBuffer, *indexBuffer, *layout);

        bitmap = this->font.getBitmap();
    }

    Text::~Text() = default;

    void Text::draw(std::shared_ptr<Render> renderer) {
        const size_t length = string.length();
        const uint8_t* cstr = reinterpret_cast<const uint8_t *>(string.c_str());
        float textureWidth = (float) font.getTextureWidth();
        float textureHeight = (float) font.getTextureHeight();

        vec2 cursor = begin;
        for (unsigned int drawCallIndex = 0; drawCallIndex < length; drawCallIndex += 64) {
            memset(vertexData, 0, sizeof(RenderChar) * 64);
            memset(indexData, 0, sizeof(unsigned int) * 6 * 64);

            unsigned int index;
            unsigned int charIndex = 0;
            for (index = 0; index < 64; ++index) {
                if (charIndex >= length) {
                    break;
                }

                //make utf-8 stuff
                //
                // ascii if char is 0XXXXXXX
                // utf-8 stuff is char is 1XXXXXXX
                //  number of 1 before 0 is number of bytes in char
                //  bytes after start byte are 10XXXXXX
                //  result is all bits in X
                //example:
                //01111111 => ascii shit do nothing
                //11000011 => start of utf-8 shit do stuff
                // because it has two 1 before the first 0 there are 2 bytes for this char
                // next byte must start with 10
                // next byte is 10110110
                // result is 00011 110110 (first 5 bits from first byte; next 6 bits from second byte)
                uint32_t charCode = cstr[charIndex];
                if(charCode & (uint32_t)BIT(7)){
                    if(charCode & (uint32_t)BIT(6)){
                        if(charCode & (uint32_t)BIT(5)){
                            if(charCode & (uint32_t)BIT(4)){
                                //should be 4 byte symbol
                                if(charIndex + 3 >= length){
                                    warning("String is not long enough for 4 byte char");
                                    ++charIndex;
                                    continue;
                                }
                                charCode = 0;
                                charCode |= (cstr[charIndex] & (uint32_t)0b00000111);
                                charCode <<= (uint32_t)6;
                                charCode |= (cstr[charIndex+1] & (uint32_t)0b00111111);
                                charCode <<= (uint32_t)6;
                                charCode |= (cstr[charIndex+2] & (uint32_t)0b00111111);
                                charCode <<= (uint32_t)6;
                                charCode |= (cstr[charIndex+2] & (uint32_t)0b00111111);
                                charIndex+=3;
                            }else{
                                //3 byte symbol
                                if(charIndex + 2 >= length){
                                    warning("String is not long enough for 3 byte char");
                                    ++charIndex;
                                    continue;
                                }
                                charCode = 0;
                                charCode |= (cstr[charIndex] & (uint32_t)0b00001111);
                                charCode <<= (uint32_t)6;
                                charCode |= (cstr[charIndex+1] & (uint32_t)0b00111111);
                                charCode <<= (uint32_t)6;
                                charCode |= (cstr[charIndex+2] & (uint32_t)0b00111111);
                                charIndex+=2;
                            }
                        }else{
                            // 2 byte symbol
                            if(charIndex + 1 >= length){
                                warning("String is not long enough for 2 byte char");
                                ++charIndex;
                                continue;
                            }
                            charCode = 0;
                            charCode |= (cstr[charIndex] & (uint32_t)0b00011111);
                            charCode <<= (uint32_t)6;
                            charCode |= (cstr[charIndex+1] & (uint32_t)0b00111111);
                            charIndex+=1;
                        }
                    }else{
                        //found multibytechar but not start
                    }
                }

                RenderChar renderChar{};
                FontCharMetric metric = font.getCharMetric(charCode);
                FontCharBitmapInformation bitmapInformation = font.getCharBitmapInformation(charCode);
                renderChar.topLeft.position.x = cursor.x + ((float) metric.horiBearingX) / 64.0f;
                renderChar.topLeft.position.y = cursor.y - ((float) metric.horiBearingY) / 64.0f;
                renderChar.topLeft.texPosition.x = (float) bitmapInformation.x / textureWidth;
                renderChar.topLeft.texPosition.y = (float) bitmapInformation.y / textureHeight;

                renderChar.topRight.position.x =
                        cursor.x + ((float) metric.horiBearingX + (float) metric.width) / 64.0f;
                renderChar.topRight.position.y = cursor.y - ((float) metric.horiBearingY) / 64.0f;
                renderChar.topRight.texPosition.x =
                        (float) (bitmapInformation.x + bitmapInformation.width) / textureWidth;
                renderChar.topRight.texPosition.y = (float) bitmapInformation.y / textureHeight;

                renderChar.bottomRight.position.x =
                        cursor.x + ((float) metric.horiBearingX + (float) metric.width) / 64.0f;
                renderChar.bottomRight.position.y =
                        cursor.y + (-(float) metric.horiBearingY + (float) metric.height) / 64.0f;
                renderChar.bottomRight.texPosition.x =
                        (float) (bitmapInformation.x + bitmapInformation.width) / textureWidth;
                renderChar.bottomRight.texPosition.y =
                        (float) (bitmapInformation.y + bitmapInformation.height) / textureHeight;

                renderChar.bottomLeft.position.x = cursor.x + ((float) metric.horiBearingX) / 64.0f;
                renderChar.bottomLeft.position.y =
                        cursor.y + (-(float) metric.horiBearingY + (float) metric.height) / 64.0f;
                renderChar.bottomLeft.texPosition.x = (float) bitmapInformation.x / textureWidth;
                renderChar.bottomLeft.texPosition.y =
                        (float) (bitmapInformation.y + bitmapInformation.height) / textureHeight;

                cursor.x += (float) metric.horiAdvance / 64.0f;

                vertexData[index] = renderChar;
                indexData[index * 6 + 0] = index * 4 + 0;
                indexData[index * 6 + 1] = index * 4 + 1;
                indexData[index * 6 + 2] = index * 4 + 2;
                indexData[index * 6 + 3] = index * 4 + 2;
                indexData[index * 6 + 4] = index * 4 + 3;
                indexData[index * 6 + 5] = index * 4 + 0;

                ++charIndex;
            }
            indexBuffer->setData(indexData, 6 * 64);
            vertexBuffer->setData(vertexData, sizeof(RenderChar) * 64);
            data.shader->setTexture("tex", *bitmap);
            data.shader->setUniform4f("u_textColor", color.r, color.g, color.b, color.a);
            renderer->setAlphaEnabled(true);
            renderer->draw(*vertexArray, *data.shader);
        }
    }

    void Text::setText(std::string str) {
        string = std::move(str);
    }

    void Text::setFont(const Font &f) {
        font = f;
        bitmap = font.getBitmap();
    }

    const std::string &Text::getText() {
        return string;
    }

    const Font &Text::getFont() {
        return font;
    }

}


