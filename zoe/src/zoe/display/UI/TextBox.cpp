//
// Created by Florian on 02.09.2020.
//

#include "TextBox.h"
#include "../../core/Application.h"
#include "UITool.h"
#include "../../core/KeyCode.h"
#include "../ComponentLayer.h"
#include "../../core/UTF.h"
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
    eventDispatcher.dispatch<CharInputEvent>([this](CharInputEvent& e) { this->onTextInput(e); });
}
void TextBox::onClick(MouseButtonPressedEvent& event) {
    setFocus(true);
}


void TextBox::onKeyPress(KeyPressedEvent& event) {
    if (!writeable || !hasFocus())
        return;
    int keyCode = event.getKeyCode();
    ///@todo add cursor
    if (multiLine) {
        warning("Multiline mode is not supported yet.");
    } else {
        ///@todo Add UTF8 support for character deletion
        if (keyCode == KEY_BACKSPACE) {
            text = text.substr(0, text.length() - 1);
        } else if (keyCode == KEY_ESCAPE) {
            setFocus(false);
        }
    }
}

void TextBox::onTextInput(CharInputEvent& event) {
    if (!writeable || !hasFocus())
        return;
    
    if (multiLine) {
        warning("Multiline mode is not supported yet.");
    } else {
        UTF::codepointToUTF8(event.getCodePoint(), [this](uint8_t ch) {
            text += (char) ch;
        });
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