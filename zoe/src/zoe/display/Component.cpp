//
// Created by Florian on 10.07.2020.
//

#include "Component.h"
#include "Game/Sprite.h"
#include "Game/WorldObject.h"
#include "Game/ModelComponent.h"
#include "UI/Button.h"
#include "NativeScriptComponent.h"
#include "ChaiScriptComponent.h"
#include "UI/TextBox.h"

namespace Zoe {

static std::shared_ptr<std::map<std::string, std::function<std::shared_ptr<BaseComponent>()>>> registeredComponents
        = std::make_shared<std::map<std::string, std::function<std::shared_ptr<BaseComponent>()>>>();

BaseComponent::BaseComponent() = default;

void BaseComponent::add(const std::shared_ptr<BaseComponent> &component) {
    if (auto ptr = component->parent.lock()) {
        warning("Component already has a parent! Component is not added!");
    } else {
        component->parent = shared_from_this();
        children.push_back(component);
    }
}

void BaseComponent::init(const XMLNode &node) {
    fill(node);
    for (const auto &xmlChild : node.children) {
        if (hasComponentConstructor(xmlChild.name)) {
            std::shared_ptr<BaseComponent> child = createComponent(xmlChild.name);
            add(child);
            child->init(xmlChild);
        } else {
            warning("Component ", xmlChild.name, " is not found");
        }
    }
    postFill();
}

void
BaseComponent::registerComponent(const std::string &name, const std::function<std::shared_ptr<BaseComponent>()> &func) {
    (*registeredComponents)[name] = func;
}

std::shared_ptr<BaseComponent> BaseComponent::createComponent(const std::string &name) {
    return (*registeredComponents)[name]();
}

bool BaseComponent::hasComponentConstructor(const std::string &name) {
    return registeredComponents->count(name);
}

void BaseComponent::draw(const Camera &camera) {
    onDraw(camera);
    for (const auto &child: children) {
        child->draw(camera);
    }
}

void BaseComponent::update(double time) {
    onUpdate(time);
    for (const auto &child: children) {
        child->update(time);
    }
}

void BaseComponent::inputEvent(Event &event) {
    onInputEvent(event);
    for (const auto &child: children) {
        child->inputEvent(event);
    }
}

void BaseComponent::init() {
    NativeScriptComponent::init();

    registerComponent<Sprite>("Sprite");
    registerComponent<ComponentGroup>("ComponentGroup");
    registerComponent<WorldObject>("WorldObject");
    registerComponent<Button>("Button");
    registerComponent<TextBox>("TextBox");
    registerComponent<ModelComponent>("Model");
    registerComponent<NativeScriptComponent>("NativeScriptComponent");
    registerComponent<ChaiScriptComponent>("ChaiScriptComponent");
}

vec3 BaseComponent::getWorldPosition() {
    if (auto ptr = parent.lock()) {
        return ptr->getWorldPosition() + position;
    } else {
        return position;
    }
}

}