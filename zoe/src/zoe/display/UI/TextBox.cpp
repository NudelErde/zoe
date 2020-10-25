//
// Created by Florian on 02.09.2020.
//

#include "TextBox.h"
#include "../../core/Application.h"
#include "UITool.h"

namespace Zoe {

TextBox::TextBox() : font(Application::getDefaultFont()) {}

void TextBox::onDraw(const Camera& camera) {
    UITool tool(camera);
    tool.setColor(color);
    vec2 pos = getWorldPosition().xy();
    tool.drawText(text, pos);
}
void TextBox::onUpdate(double time) {

}
void TextBox::onInputEvent(Event& event) {

}
void TextBox::fill(const XMLNode& node) {
    std::string fontName{};
    int size = -1;
    color.w = 1;

    //position
    if (auto iter = node.attributes.find("x"); iter != node.attributes.end()) {
        position.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("y"); iter != node.attributes.end()) {
        position.y = std::stof(iter->second);
    }

    //color
    if (auto iter = node.attributes.find("r"); iter != node.attributes.end()) {
        color.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("g"); iter != node.attributes.end()) {
        color.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("b"); iter != node.attributes.end()) {
        color.z = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("a"); iter != node.attributes.end()) {
        color.w = std::stof(iter->second);
    }
    //content
    if (auto iter = node.attributes.find("text"); iter != node.attributes.end()) {
        text = iter->second;
    }
    if (auto iter = node.attributes.find("font"); iter != node.attributes.end()) {
        fontName = iter->second;
    }
    if (auto iter = node.attributes.find("size"); iter != node.attributes.end()) {
        size = std::stoi(iter->second);
    }

    if (size == -1) {
        size = 30;
    }

    if (!fontName.empty()) {
        Path fontPath(fontName);
        if (fontPath.isFile()) {
            font = Font(fontPath, size);
        } else {
            warning("Font ", fontPath.getAbsolutePath(), " does not name a File. Fallback to default Font");
        }
    } else if (size != font.getSize()) {
        font = Font(font, size);
    }
}
void TextBox::postFill() {

}
}