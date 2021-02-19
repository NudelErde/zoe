//
// Created by Florian on 18.02.2021.
//

#include "Image.h"
#include "UITool.h"
#include "../../core/Application.h"

namespace Zoe {

Image::Image() = default;
void Image::setSource(const File& source) {
    *imageSource = source;
    texture = Application::getContext().getTexture(source);
}
const File& Image::getSource() const {
    return *imageSource;
}
void Image::onDraw(const Camera& camera) {
    if(!isVisible() || !imageSource)
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

}
void Image::postFill() {

}
}