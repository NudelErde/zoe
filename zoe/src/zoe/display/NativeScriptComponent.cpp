//
// Created by Florian on 29.08.2020.
//

#include "NativeScriptComponent.h"

namespace Zoe {

static std::shared_ptr<std::map<std::string, std::function<std::unique_ptr<NativeScript>()>>> nativeScriptCreatorMap;

void NativeScriptComponent::init() {
    nativeScriptCreatorMap = std::make_shared<std::map<std::string, std::function<std::unique_ptr<NativeScript>()>>>();
}

void NativeScriptComponent::registerNativeScript(const std::string& scriptName,
                                                 const std::function<std::unique_ptr<NativeScript>()>& func) {
    nativeScriptCreatorMap->operator[](scriptName) = func;
}
std::unique_ptr<NativeScript> NativeScriptComponent::getNativeScript(const std::string& scriptName) {
    if (auto iter = nativeScriptCreatorMap->find(scriptName); iter != nativeScriptCreatorMap->end()) {
        return (*iter).second();
    } else {
        warning("Tried to create non existing native script ", scriptName);
        return std::make_unique<NativeScript>();
    }
}
bool NativeScriptComponent::hasNativeScript(const std::string& scriptName) {
    return nativeScriptCreatorMap->count(scriptName);
}

void NativeScriptComponent::onDraw(const Camera& camera) {}

void NativeScriptComponent::onUpdate(double time) {
    if (!script) {
        if (NativeScriptComponent::hasNativeScript(scriptKey)) {
            script = NativeScriptComponent::getNativeScript(scriptKey);
            script->component = getParent();
            script->scriptComponent = std::static_pointer_cast<NativeScriptComponent>(shared_from_this());
            script->onInit();
        }
    } else {
        script->onUpdate(time);
    }
}

void NativeScriptComponent::onInputEvent(Event& event) {}
void NativeScriptComponent::fill(const XMLNode& node) {
    if (node.attributes.count("script")) {
        scriptKey = node.attributes.at("script");
        if (NativeScriptComponent::hasNativeScript(scriptKey)) {
            script = NativeScriptComponent::getNativeScript(scriptKey);
        }
    }
}
void NativeScriptComponent::postFill() {
    if (script) {
        script->component = getParent();
        script->scriptComponent = std::static_pointer_cast<NativeScriptComponent>(shared_from_this());
        script->onInit();
    }
}
void NativeScriptComponent::onActivation() {
    script->onActivation();
}
void NativeScriptComponent::onCollision(double delta, const std::function<void()>& resolve) {
    script->onCollision(delta, resolve);
}

}