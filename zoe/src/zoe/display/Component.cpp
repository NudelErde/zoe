//
// Created by Florian on 10.07.2020.
//

#include "Component.h"
#include "Game/Sprite.h"
#include "Game/WorldObject.h"
#include "UI/Button.h"

namespace Zoe{

static std::map<std::string, std::function<std::shared_ptr<BaseComponent>()>> registeredComponents;

//public
BaseComponent::BaseComponent() = default;

void BaseComponent::add(const std::shared_ptr<BaseComponent>& component) {
    devDebug("Add component");
    if(auto ptr = component->parent.lock()){
        warning("Component already has a parent! Component is not added!");
    }else{
        devDebug("Component has no parent");
        component->parent = shared_from_this();
        devDebug("Created pointer from this and assigned to child->parent");
        children.push_back(component);
        devDebug("Push child in children vector");
    }
    devDebug("Added component");
}

//private
void BaseComponent::init(const XMLNode &node) {
    fill(node);
    for (const auto &xmlChild : node.children) {
        std::shared_ptr<BaseComponent> child = createComponent(xmlChild.name);
        add(child);
        child->init(xmlChild);
    }
    postFill();
}

void BaseComponent::registerComponent(const std::string &name, const std::function<std::shared_ptr<BaseComponent>()>& func) {
    registeredComponents[name] = func;
}

std::shared_ptr<BaseComponent> BaseComponent::createComponent(const std::string &name) {
    return registeredComponents[name]();
}

void BaseComponent::draw(const Camera& camera) {
    onDraw(camera);
    for(const auto& child: children){
        child->draw(camera);
    }
}

void BaseComponent::update(double time) {
    onUpdate(time);
    for(const auto& child: children){
        child->update(time);
    }
}

void BaseComponent::inputEvent(Event &event) {
    onInputEvent(event);
    for(const auto& child: children){
        child->inputEvent(event);
    }
}

void BaseComponent::init() {
    registerComponent<Sprite>("Sprite");
    registerComponent<ComponentGroup>("ComponentGroup");
    registerComponent<WorldObject>("WorldObject");
    registerComponent<Button>("Button");
}

vec3 BaseComponent::getWorldPosition() {
    if(auto ptr = parent.lock()){
        return ptr->getWorldPosition()+position;
    }else{
        return position;
    }
}

}