//
// Created by Florian on 29.08.2020.
//

#include "ChaiScriptComponent.h"

#ifndef CHAISCRIPT_NO_THREADS
#define CHAISCRIPT_NO_THREADS
#endif

#include <chaiscript/chaiscript.hpp>

//TODO: connection between c++ and chaiscript (chaiscript module?)
namespace Zoe {

struct ChaiScriptInstance {
    chaiscript::ChaiScript chai;
    std::function<void(double)> updateFunction;
    std::function<void()> initFunction;
};

ChaiScriptComponent::ChaiScriptComponent() = default;

ChaiScriptComponent::ChaiScriptComponent(ChaiScriptComponent &&) noexcept = default;
ChaiScriptComponent &ChaiScriptComponent::operator=(ChaiScriptComponent &&) noexcept = default;
ChaiScriptComponent::~ChaiScriptComponent() = default;

void ChaiScriptComponent::onDraw(const Camera &camera) {}

void ChaiScriptComponent::onUpdate(double time) {
    script->updateFunction(time);
}
void ChaiScriptComponent::onInputEvent(Event &event) {}

void ChaiScriptComponent::fill(const XMLNode &node) {
    if (node.attributes.count("script")) {
        Path path(node.attributes.at("script"));
        if (path.isFile()) {
            size_t size;
            std::unique_ptr<uint8_t[]> content = File(path).getContent(&size);
            std::string str((char *) content.get(), size);
            script->chai.eval(str);
            try {
                script->updateFunction = script->chai.eval<std::function<void(double)>>("onUpdate");
            } catch (...) {
                script->updateFunction = [](double){};
            }
            try {
                script->initFunction = script->chai.eval<std::function<void()>>("onInit");
            } catch (...) {
                script->initFunction = [](){};
            }
        } else {
            warning("Could not load script: ", path.getAbsolutePath(), " does not name a File");
        }
    }
}
void ChaiScriptComponent::postFill() {
    script->initFunction();
}
void ChaiScriptComponent::onActivation() {

}
void ChaiScriptComponent::onCollision(double delta, const std::function<void()>& resolve) {

}

}
