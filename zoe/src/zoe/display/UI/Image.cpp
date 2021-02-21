//
// Created by Florian on 18.02.2021.
//

#include "Image.h"
#include "UITool.h"
#include "../../core/Application.h"

namespace Zoe {

Image::Image() = default;
void Image::setSource(const File& source) {
    imageSource = std::make_unique<File>(source);
    texture = Application::getContext().getTexture(source);
}
const File& Image::getSource() const {
    return *imageSource;
}
void Image::onDraw(const Camera& camera) {
    if (!isVisible() || !imageSource)
        return;
    UITool t(camera);
    vec2 pos = getWorldPosition().xy();
    t.drawImage(texture, pos, size);
}
void Image::onUpdate(double time) {

}
void Image::onInputEvent(Event& event) {

}
void Image::fill(const XMLNode& node) {
    if (auto iter = node.attributes.find("x"); iter != node.attributes.end()) {
        position.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("y"); iter != node.attributes.end()) {
        position.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("width"); iter != node.attributes.end()) {
        size.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("height"); iter != node.attributes.end()) {
        size.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("src"); iter != node.attributes.end()) {
        setSource(iter->second);
    }
}
void Image::postFill() {

}
}