//
// Created by Florian on 18.07.2020.
//

#include "UITool.h"

#include <utility>
#include <cstring>

#include "../../core/Application.h"

namespace Zoe {

static std::unique_ptr<Font> defaultFont;
static std::shared_ptr<VertexBuffer> rectVertexBuffer;
static std::shared_ptr<IndexBuffer> rectIndexBuffer;
static std::shared_ptr<VertexArray> rectVertexArray;
static std::shared_ptr<Shader> imageShader;
static std::shared_ptr<Shader> rectangleShader;
static std::shared_ptr<Shader> ovalShader;
static std::shared_ptr<Shader> textShader;

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

void UITool::init() {
    defaultFont = std::make_unique<Font>(File("virtual/zoe/display/Roboto-Regular.ttf"), 30);
    rectVertexBuffer = Application::getContext().getVertexBuffer(false);
    rectIndexBuffer = Application::getContext().getIndexBuffer(false);
    rectVertexArray = Application::getContext().getVertexArray();
    float rectVertexData[]{
            0, 0,
            1, 0,
            1, 1,
            0, 1
    };
    unsigned int rectIndexData[]{
            0, 1, 2, 2, 3, 0
    };
    rectVertexBuffer->setData(rectVertexData, sizeof(float) * 8);
    rectIndexBuffer->setData(rectIndexData, 6);
    std::shared_ptr<VertexBufferLayout> rectLayout = Application::getContext().getVertexBufferLayout();
    rectLayout->push_float(2);
    rectVertexArray->set(rectVertexBuffer, rectIndexBuffer, rectLayout);
    imageShader = Application::getContext().getShader(File("virtual/zoe/display/ui/Image.glsl"));
    rectangleShader = Application::getContext().getShader(File("virtual/zoe/display/ui/Rectangle.glsl"));
    ovalShader = Application::getContext().getShader(File("virtual/zoe/display/ui/Oval.glsl"));
    textShader = Application::getContext().getShader(File("virtual/zoe/display/ui/Text.glsl"));
}

UITool::UITool(const Camera &camera) : font(*defaultFont), color(vec4({0, 0, 0, 1})),
                                       render(camera.getRender()),
                                       projectionView(camera.getProjectionMatrix() * camera.getViewMatrix()) {
    textVertexBuffer = Application::getContext().getVertexBuffer(true);
    textIndexBuffer = Application::getContext().getIndexBuffer(true);
    textVertexArray = Application::getContext().getVertexArray();

    RenderChar textVertexData[64];
    unsigned int textIndexData[6 * 64];
    memset(textVertexData, 0, sizeof(RenderChar) * 64);
    memset(textIndexData, 0, sizeof(unsigned int) * 6 * 64);

    textVertexBuffer->setData(textVertexData, sizeof(RenderChar) * 64);
    textIndexBuffer->setData(textIndexData, 6 * 64);
    std::shared_ptr<VertexBufferLayout> layout = Application::getContext().getVertexBufferLayout();
    layout->push_float(2);
    layout->push_float(2);
    textVertexArray->set(textVertexBuffer, textIndexBuffer, layout);
    textBitmap = this->font.getBitmap();
}

void UITool::setColor(const vec4& c) {
    color = c;
}

void UITool::setFont(const Font& f) {
    font = f;
}

const vec4 &UITool::getColor() {
    return color;
}

const Font &UITool::getFont() {
    return font;
}

void UITool::drawImage(const std::shared_ptr<Texture> &image, const vec2 &pos, const vec2 &size) {
    mat4x4 modelViewProjection = projectionView * translate3D(pos.x-800,-pos.y+450,0) * scale3D(size.x,-size.y,1);
    imageShader->setUniform4m("ModelViewProjection", modelViewProjection);
    imageShader->setTexture("u_image", *image);
    render->draw(*rectVertexArray, *imageShader);
}

void UITool::drawRectangle(const vec2 &pos, const vec2 &size) {
    mat4x4 modelViewProjection = projectionView * translate3D(pos.x-800,-pos.y+450,0) * scale3D(size.x,-size.y,1);
    rectangleShader->setUniform4m("ModelViewProjection", modelViewProjection);
    rectangleShader->setUniform4f("Color", color.x, color.y, color.z, color.w);
    render->draw(*rectVertexArray, *rectangleShader);
}

void UITool::drawOval(const vec2 &pos, const vec2 &size) {
    mat4x4 modelViewProjection = projectionView * translate3D(pos.x-800,-pos.y+450,0) * scale3D(size.x,-size.y,1);
    ovalShader->setUniform4m("ModelViewProjection", modelViewProjection);
    ovalShader->setUniform4f("Color", color.x, color.y, color.z, color.w);
    render->draw(*rectVertexArray, *ovalShader);
}

void UITool::drawText(const std::string & string, const vec2 &pos) {
    RenderChar textVertexData[64];
    unsigned int textIndexData[6 * 64];

    const size_t length = string.length();
    const auto* cstr = reinterpret_cast<const uint8_t *>(string.c_str());
    auto textureWidth = (float) font.getTextureWidth();
    auto textureHeight = (float) font.getTextureHeight();

    mat4x4 modelViewProjection = projectionView * translate3D(-800,450,0)*scale3D(1,-1,1);
    textShader->setUniform4m("ModelViewProjection", modelViewProjection);

    vec2 cursor = pos;
    for (unsigned int drawCallIndex = 0; drawCallIndex < length; drawCallIndex += 64) {
        memset(textVertexData, 0, sizeof(RenderChar) * 64);
        memset(textIndexData, 0, sizeof(unsigned int) * 6 * 64);

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

            textVertexData[index] = renderChar;
            textIndexData[index * 6 + 0] = index * 4 + 0;
            textIndexData[index * 6 + 1] = index * 4 + 1;
            textIndexData[index * 6 + 2] = index * 4 + 2;
            textIndexData[index * 6 + 3] = index * 4 + 2;
            textIndexData[index * 6 + 4] = index * 4 + 3;
            textIndexData[index * 6 + 5] = index * 4 + 0;

            ++charIndex;
        }
        textIndexBuffer->setData(textIndexData, 6 * 64);
        textVertexBuffer->setData(textVertexData, sizeof(RenderChar) * 64);

        textShader->setTexture("tex", *textBitmap);
        textShader->setUniform4f("u_textColor", color.x, color.y, color.z, color.w);
        render->setAlphaEnabled(true);
        render->draw(*textVertexArray, *textShader);
    }
}

}