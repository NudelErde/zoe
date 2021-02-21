//
// Created by Florian on 21.02.2021.
//

#include "Rectangle.h"
#include "UITool.h"

namespace Zoe {

Rectangle::Rectangle() = default;


void Rectangle::onDraw(const Camera& camera) {
    if(!isVisible())
        return;
    UITool t(camera);
    t.setColor(color);
    t.drawRectangle(getWorldPosition().xy(), size);
}
void Rectangle::onUpdate(double time) {

}
void Rectangle::onInputEvent(Event& event) {

}
void Rectangle::fill(const XMLNode& node) {
    if (auto iter = node.attributes.find("x"); iter != node.attributes.end()) {
        position.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("y"); iter != node.attributes.end()) {
        position.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("z"); iter != node.attributes.end()) {
        position.z = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("width"); iter != node.attributes.end()) {
        size.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("height"); iter != node.attributes.end()) {
        size.y = std::stof(iter->second);
    }
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
    } else {
        color.w = 1;
    }
}
void Rectangle::postFill() {

}
}