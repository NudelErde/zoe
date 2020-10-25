//
// Created by Florian on 23.07.2020.
//

#include "Button.h"
#include "../ComponentLayer.h"
#include "UITool.h"
#include "../../core/Application.h"

namespace Zoe {

Button::Button() : font(Application::getDefaultFont()) {
    onClick = []() {};
}

void Button::setClickHandler(const std::function<void()>& handler) {
    onClick = handler;
}

void Button::onDraw(const Camera& camera) {
    UITool tool(camera);
    tool.setColor(backgroundColor);
    vec2 pos = getWorldPosition().xy();
    tool.drawRectangle(pos, size);
    tool.setColor(textColor);
    tool.setFont(font);
    vec2 textPos = pos;
    textPos.y += size.y; /// @todo calculate this so that the text is centered
    tool.drawText(text, textPos);
}

void Button::onUpdate(double time) {

}

void Button::onInputEvent(Event& event) {
    EventDispatcher ed(event);
    ed.dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent& e) { this->onMouseClick(e); });
}

void Button::fill(const XMLNode& node) {
    std::string fontName{};
    int fontSize = -1;

    //text color
    ///@todo text_something and background_something can't be the right solution because stupid names and stuff. Change this so that it is good
    textColor.w = 1;
    if (auto iter = node.attributes.find("text_r"); iter != node.attributes.end()) {
        textColor.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("text_g"); iter != node.attributes.end()) {
        textColor.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("text_b"); iter != node.attributes.end()) {
        textColor.z = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("text_a"); iter != node.attributes.end()) {
        textColor.w = std::stof(iter->second);
    }

    //background color
    backgroundColor.w = 1;
    if (auto iter = node.attributes.find("background_r"); iter != node.attributes.end()) {
        backgroundColor.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("background_g"); iter != node.attributes.end()) {
        backgroundColor.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("background_b"); iter != node.attributes.end()) {
        backgroundColor.z = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("background_a"); iter != node.attributes.end()) {
        backgroundColor.w = std::stof(iter->second);
    }

    //position
    if (node.attributes.count("x") > 0) {
        position.x = std::stof(node.attributes.at("x"));
    }
    if (node.attributes.count("y") > 0) {
        position.y = std::stof(node.attributes.at("y"));
    }
    position.z = 0;

    //size
    if (node.attributes.count("width") > 0) {
        size.x = std::stof(node.attributes.at("width"));
    }
    if (node.attributes.count("height") > 0) {
        size.y = std::stof(node.attributes.at("height"));
    }

    //content
    if (auto iter = node.attributes.find("text"); iter != node.attributes.end()) {
        text = iter->second;
    }
    if (auto iter = node.attributes.find("font"); iter != node.attributes.end()) {
        fontName = iter->second;
    }
    if (auto iter = node.attributes.find("size"); iter != node.attributes.end()) {
        fontSize = std::stoi(iter->second);
    }

    if (fontSize == -1) {
        fontSize = 30;
    }

    if (!fontName.empty()) {
        Path fontPath(fontName);
        if (fontPath.isFile()) {
            font = Font(fontPath, fontSize);
        } else {
            warning("Font ", fontPath.getAbsolutePath(), " does not name a File. Fallback to default Font");
        }
    } else if (fontSize != font.getSize()) {
        font = Font(font, fontSize);
    }
}

void Button::postFill() {
    ///@todo search for script child => check if onClick function is in script
}

void Button::onMouseClick(MouseButtonReleasedEvent& mbre) {
    /**
     * @todo check if element is in focus and active and visible
     * @todo why the fuck isn't this showing in doxygen :c
     */
    vec2 position = getWorldPosition().xy();
    vec2 pos = ComponentLayer::getMousePosition();
    if (pos.x > position.x && pos.x < position.x + size.x
        && pos.y > position.y && pos.y < position.y + size.y) {
        onClick();
    }
}

const vec4& Button::getBackgroundColor() const {
    return backgroundColor;
}

void Button::setBackgroundColor(const vec4& _color) {
    backgroundColor = _color;
}

const vec4& Button::getTextColor() const {
    return textColor;
}

void Button::setTextColor(const vec4& _color) {
    textColor = _color;
}

const std::string& Button::getText() {
    return text;
}

void Button::setText(const std::string& _text) {
    text = _text;
}

const Font& Button::getFont() {
    return font;
}

void Button::setFont(const Font& _font) {
    font = _font;
}

}
