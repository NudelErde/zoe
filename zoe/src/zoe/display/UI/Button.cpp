//
// Created by Florian on 23.07.2020.
//

#include "Button.h"
#include "../../core/Input.h"
#include "../ComponentLayer.h"
#include "UITool.h"

namespace Zoe {

Button::Button() {
    onClick = [](){};
}

void Button::setClickHandler(const std::function<void()> &handler) {
    onClick = handler;
}

void Button::onDraw(const Camera &camera) {
    UITool tool(camera);
    tool.setColor(vec4({1,0,0,1}));
    vec2 pos = getWorldPosition().xy;
    tool.drawRectangle(pos, size);
}

void Button::onUpdate(double time) {

}

void Button::onInputEvent(Event &event) {
    EventDispatcher ed(event);
    ed.dispatch<MouseButtonReleasedEvent>([this](MouseButtonReleasedEvent &e) { this->onMouseClick(e); });
}

void Button::fill(const XMLNode &node) {
    if (node.attributes.count("x") > 0) {
        position.x = std::stof(node.attributes.at("x"));
    }
    if (node.attributes.count("y") > 0) {
        position.y = std::stof(node.attributes.at("y"));
    }
    position.z = 0;
    if (node.attributes.count("width") > 0) {
        size.x = std::stof(node.attributes.at("width"));
    }
    if (node.attributes.count("height") > 0) {
        size.y = std::stof(node.attributes.at("height"));
    }
}

void Button::postFill() {
    //TODO: search for script child => check if onClick function is in script
}

void Button::onMouseClick(MouseButtonReleasedEvent &mbre) {
    //TODO: check if element is in focus and active and visible
    vec2 position = getWorldPosition().xy;
    vec2 pos = ComponentLayer::getMousePosition();
    if (pos.x > position.x && pos.x < position.x + size.x
        && pos.y > position.y && pos.y < position.y + size.y) {
        onClick();
    }
}

}
