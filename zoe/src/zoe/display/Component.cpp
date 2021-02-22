//
// Created by Florian on 10.07.2020.
//

#include "Component.h"
#include "Game/Sprite.h"
#include "Game/WorldObject.h"
#include "Game/ModelComponent.h"
#include "UI/Button.h"
#include "UI/TextBox.h"
#include "UI/Image.h"
#include "UI/Rectangle.h"
#include "NativeScriptComponent.h"
#include "ChaiScriptComponent.h"
#include "Physics/PhysicsGroup.h"
#include "ComponentLayer.h"
#include "Physics/BoxCollider.h"

namespace Zoe {

static std::shared_ptr<std::map<std::string, std::function<std::shared_ptr<BaseComponent>()>>> registeredComponents
        = std::make_shared<std::map<std::string, std::function<std::shared_ptr<BaseComponent>()>>>();

void BaseComponent::add(const std::shared_ptr<BaseComponent>& component) {
    if (auto ptr = component->parent.lock()) {
        warning("Component already has a parent! Component is not added!");
    } else {
        component->parent = shared_from_this();
        children.push_back(component);
        std::vector<std::shared_ptr<BaseComponent>> components;
        components.push_back(component);
        for (const auto& comp : components) {
            comp->layer = layer;
            components.insert(components.end(), comp->children.begin(), comp->children.end());
        }
    }
}

std::shared_ptr<BaseComponent> BaseComponent::componentByXML(const XMLNode& node) {
    if (hasComponentConstructor(node.name)) {
        std::shared_ptr<BaseComponent> cmp = createComponent(node.name);
        cmp->init(node);
        return cmp;
    } else {
        return {};
    }
}

void BaseComponent::commonAttributeFill(const XMLNode& node) {
    if(auto iter = node.attributes.find("id"); iter != node.attributes.end()) {
        id = iter->second;
    }
    if (auto iter = node.attributes.find("x"); iter != node.attributes.end()) {
        position.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("y"); iter != node.attributes.end()) {
        position.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("z"); iter != node.attributes.end()) {
        position.z = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("visible"); iter != node.attributes.end()) {
        setVisible(iter->second == "true");
    } else {
        setVisible(true);
    }
}

void BaseComponent::init(const XMLNode& node) {
    commonAttributeFill(node);
    fill(node);
    for (const auto& xmlChild : node.children) {
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
BaseComponent::registerComponent(const std::string& name, const std::function<std::shared_ptr<BaseComponent>()>& func) {
    (*registeredComponents)[name] = func;
}

std::shared_ptr<BaseComponent> BaseComponent::createComponent(const std::string& name) {
    return (*registeredComponents)[name]();
}

bool BaseComponent::hasComponentConstructor(const std::string& name) {
    return registeredComponents->count(name);
}

void BaseComponent::draw(const Camera& camera) {
    if(!isVisible())
        return;
    onDraw(camera);
    for (const auto& child: children) {
        child->draw(camera);
    }
}

void BaseComponent::update(double time) {
    onUpdate(time);
    for (const auto& child: children) {
        child->update(time);
    }
}

void BaseComponent::physicsUpdate(double time) {
    onPhysicsUpdate(time);
    for (const auto& child: children) {
        child->physicsUpdate(time);
    }
}

void BaseComponent::inputEvent(Event& event) {
    onInputEvent(event);
    for (const auto& child: children) {
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
    registerComponent<Camera2D>("Camera2D");
    registerComponent<Camera3D>("Camera3D");
    registerComponent<PhysicsGroup>("PhysicsGroup");
    registerComponent<BoxCollider>("BoxCollider");
    registerComponent<Image>("Image");
    registerComponent<Rectangle>("Rectangle");
}
vec3 BaseComponent::getWorldPosition() const {
    if (auto ptr = parent.lock()) {
        return ptr->getWorldPosition() + position;
    } else {
        return position;
    }
}
bool BaseComponent::hasFocus() const {
    return isFocused;
}
void BaseComponent::setFocus(bool val) {
    if (val == isFocused)
        return;
    if(auto layerPtr = layer.lock()) {
        if (val) {
            if (auto ptr = layerPtr->getFocusedObject().lock()) {
                ptr->isFocused = false;
            }
            layerPtr->setFocusedObject(weak_from_this());
        } else {
            layerPtr->setFocusedObject(std::weak_ptr<BaseComponent>());
        }
        isFocused = val;
    }
}
std::shared_ptr<BaseComponent> BaseComponent::getChildByID(const std::string& componentID)  {
    std::vector<std::shared_ptr<BaseComponent>> childVector = children;
    for (const auto& child: childVector) {
        if(child->id == componentID)
            return child;
        const auto& grandchildren = child->getChildren();
        childVector.insert(childVector.end(), grandchildren.begin(), grandchildren.end());
    }
    return std::shared_ptr<BaseComponent>();
}
void BaseComponent::onPhysicsUpdate(double delta) {}

}