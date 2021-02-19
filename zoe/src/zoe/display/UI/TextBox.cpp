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
    if(!isVisible())
        return;
    UITool tool(camera);
    tool.setColor(color);
    vec2 pos = getWorldPosition().xy();
    tool.drawText(text, pos);

    if (!writeable || !hasFocus())
        return;

    if (multiLine) {
        warning("Multiline mode is not supported yet.");
    } else {
        int cursorIndex = cursorX;
        vec2 cursorPos = pos;
        text.asUTF32([&](uint32_t ch) {
            if (cursorIndex <= 0) {
                return;
            }
            cursorPos.x += (float) font.getCharMetric(ch).horiAdvance / 64.0f;
            --cursorIndex;
        });
        tool.drawRectangle(cursorPos, vec2((float) font.getSize() / 2.5f, (float) font.getSize() / 10.0f));
    }
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
    if (multiLine) {
        warning("Multiline mode is not supported yet.");
    } else {
        if (keyCode == KEY_BACKSPACE) {
            if (cursorX > 0) {
                --cursorX;
                text.remove(cursorX);
            }
        } else if (keyCode == KEY_DELETE) {
            text.remove(cursorX);
        } else if (keyCode == KEY_ESCAPE) {
            setFocus(false);
        } else if (keyCode == KEY_LEFT) {
            --cursorX;
            if (cursorX < 0) {
                cursorX = 0;
            }
        } else if (keyCode == KEY_RIGHT) {
            ++cursorX;
            auto size = text.charCount();
            if (cursorX > size) {
                cursorX = (int) size;
            }
        } else if (keyCode == KEY_END) {
            cursorX = (int) text.charCount();
        } else if (keyCode == KEY_HOME) {
            cursorX = 0;
        }
    }
}

void TextBox::onTextInput(CharInputEvent& event) {
    if (!writeable || !hasFocus())
        return;

    if (multiLine) {
        warning("Multiline mode is not supported yet.");
    } else {
        text.insert(cursorX, event.getCodePoint());
        ++cursorX;
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
        setText(iter->second);
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
            warningf("Font {} does not name a File. Fallback to default Font", fontPath.getAbsolutePath());
        }
    } else if (size != font.getSize()) {
        font.setSize(size);
    }
}
void TextBox::postFill() {

}
void TextBox::setText(const std::string& str) {
    text = str;
    cursorX = (int) text.charCount();
}
void TextBox::setText(const UTF8String& str) {
    text = str;
    cursorX = (int) text.charCount();
}
bool TextBox::isWriteable() const {
    return writeable;
}
void TextBox::setWriteable(bool writeable) {
    TextBox::writeable = writeable;
}
}