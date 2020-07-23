//
// Created by Florian on 10.07.2020.
//

#include "Sprite.h"
namespace Zoe{

void Sprite::onDraw(const Camera& camera) {}

void Sprite::onUpdate(double time) {}

void Sprite::onInputEvent(Event &event) {}

void Sprite::fill(const XMLNode &node) {
    if(node.attributes.count("x")>0){
        position.x = std::stof(node.attributes.at("x"));
    }
    if(node.attributes.count("y")>0){
        position.y = std::stof(node.attributes.at("y"));
    }
    if(node.attributes.count("z")>0){
        position.z = std::stof(node.attributes.at("z"));
    }
}

void Sprite::postFill() {

}
}