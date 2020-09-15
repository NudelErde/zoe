//
// Created by Florian on 29.08.2020.
//

#pragma once

#include "../core/Core.h"
#include "Component.h"
#include <functional>

namespace Zoe {

class NativeScriptComponent;

class DLL_PUBLIC NativeScript {
public:
    virtual void onUpdate(const double&) {}
    virtual void onInit() {}

    std::weak_ptr<BaseComponent> component{};
    std::weak_ptr<NativeScriptComponent> scriptComponent{};
};

class DLL_PUBLIC NativeScriptComponent : public BaseComponent {
public:
    static void init();

    static void registerNativeScript(const std::string &scriptName, const std::function<std::unique_ptr<NativeScript>()>&);

    template<typename ScriptClass>
    static inline void registerNativeScript(const std::string &scriptName) {
        registerNativeScript(scriptName, []() -> std::unique_ptr<NativeScript> {
            return std::make_unique<ScriptClass>();
        });
    }

    static std::unique_ptr<NativeScript> getNativeScript(const std::string &scriptName);

    static bool hasNativeScript(const std::string& scriptName);

public:
    NativeScriptComponent() = default;
    NativeScriptComponent(NativeScriptComponent &&) noexcept = default;
    NativeScriptComponent &operator=(NativeScriptComponent &&) noexcept = default;
    NativeScriptComponent(const NativeScriptComponent &) = delete;
    NativeScriptComponent &operator=(const NativeScriptComponent &) = delete;
    ~NativeScriptComponent() = default;
protected:
    void onDraw(const Camera &camera) override;
    void onUpdate(double time) override;
    void onInputEvent(Event &event) override;
    void fill(const XMLNode &node) override;
    void postFill() override;
private:
    std::unique_ptr<NativeScript> script;
    std::string scriptKey;
};

}


