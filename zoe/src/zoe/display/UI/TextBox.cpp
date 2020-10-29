//
// Created by Florian on 02.09.2020.
//

#include "TextBox.h"
#include "../../core/Application.h"
#include "UITool.h"
#include "../../core/KeyCode.h"
#include "../ComponentLayer.h"
#include <functional>

namespace Zoe {

TextBox::TextBox() = default;

void TextBox::onDraw(const Camera& camera) {
    UITool tool(camera);
    tool.setColor(color);
    vec2 pos = getWorldPosition().xy();
    tool.drawText(text, pos);
}
void TextBox::onUpdate(double time) {

}
void TextBox::onInputEvent(Event& event) {
    EventDispatcher eventDispatcher(event);
    eventDispatcher.dispatch<MouseButtonPressedEvent>([this](MouseButtonPressedEvent& e) { this->onClick(e); });
    eventDispatcher.dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) { this->onKeyPress(e); });
}
void TextBox::onClick(MouseButtonPressedEvent& event) {
    setFocus(true);
}
void TextBox::onKeyPress(KeyPressedEvent& event) {
    if (!writeable || !hasFocus())
        return;
    int keyCode = event.getKeyCode();
    if (multiLine) {
        warning("Multiline mode is not supported yet.");
    } else {
        ///@todo rework after KeyEvent contain the pressed char. (KeyLayout stuff, pls lib do this work for me :c)
        if (keyCode == KEY_BACKSPACE) {
            text = text.substr(0, text.length() - 1);
        } else if (keyCode >= KEY_A && keyCode <= KEY_Z) {
            if (ComponentLayer::isKeyPressed(KEY_LEFT_SHIFT) || ComponentLayer::isKeyPressed(KEY_RIGHT_SHIFT)) {
                text += ((char) keyCode);
            } else {
                text += ((char) ((unsigned int) keyCode | 0b00100000u));
            }
        } else if (keyCode == KEY_ESCAPE) {
            setFocus(false);
        } else if (keyCode == KEY_SPACE) {
            text += " ";
        } else if (keyCode >= KEY_0 && keyCode <= KEY_9) {
            text += ((char) keyCode);
        } else if (keyCode >= KEY_KP_0 && keyCode <= KEY_KP_9) {
            text += (char)(keyCode - KEY_KP_0 + KEY_0);
        }
    }
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

    if (auto iter = node.attributes.find("writeable"); iter != node.attributes.end()) {
        writeable = iter->second == "true";
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
        font.setSize(size);
    }
}
void TextBox::postFill() {

}
}