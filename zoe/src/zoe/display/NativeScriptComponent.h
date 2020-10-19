//
// Created by Florian on 29.08.2020.
//

#pragma once

#include "../core/Core.h"
#include "Component.h"
#include <functional>

namespace Zoe {

class NativeScriptComponent;

/**
 * The native script. Inherit this class to create a c++ script.
 */
class DLL_PUBLIC NativeScript {
public:
    virtual void onUpdate(const double&) {}
    virtual void onInit() {}

    std::weak_ptr<BaseComponent> component{};
    std::weak_ptr<NativeScriptComponent> scriptComponent{};
};

/**
 * A native script is used to script in c++.
 */
class DLL_PUBLIC NativeScriptComponent : public BaseComponent {
public:
    /**
     * Initialise the NativeScript system.
     */
    static void init();

    /**
     * Register a function that creates NativeScript objects with a specified name.
     * @param scriptName the specified name
     * @param function the function
     */
    static void registerNativeScript(const std::string &scriptName, const std::function<std::unique_ptr<NativeScript>()>& function);

    /**
     * Register a native script class with a specified name
     * @tparam ScriptClass the script class
     * @param scriptName the name of the script
     */
    template<typename ScriptClass>
    static inline void registerNativeScript(const std::string &scriptName) {
        registerNativeScript(scriptName, []() -> std::unique_ptr<NativeScript> {
            return std::make_unique<ScriptClass>();
        });
    }

    /**
     * Returns a script instance for a specified script name.
     * If there is no script with the specified name, an empty unique_ptr is returned and a warning is printed.
     * @param scriptName the script name
     * @return a script instance
     */
    static std::unique_ptr<NativeScript> getNativeScript(const std::string &scriptName);

    /**
     * Returns `true` if the specified script name exists.
     * @param scriptName the specified script name
     * @returns `true` if the script exists
     */
    static bool hasNativeScript(const std::string& scriptName);

public:
    /**
     * Creates an empty NativeScriptComponent.
     */
    NativeScriptComponent() = default;

    /**
     * Defines a default move constructor.
     */
    NativeScriptComponent(NativeScriptComponent &&) noexcept = default;

    /**
     * Defines a default move assign.
     * @return the result
     */
    NativeScriptComponent &operator=(NativeScriptComponent &&) noexcept = default;

    /**
     * Deletes the copy constructor.
     */
    NativeScriptComponent(const NativeScriptComponent &) = delete;

    /**
     * Deletes the copy assign.
     * @return the result
     */
    NativeScriptComponent &operator=(const NativeScriptComponent &) = delete;

    /**
     * Destructs the NativeScriptComponent.
     */
    ~NativeScriptComponent() = default;
protected:

    /**
     * Draws this component on the specified camera.
     * Does nothing.
     * @param camera the specified camera
     */
    void onDraw(const Camera &camera) override;

    /**
     * Calls the onUpdate methode of the native script with the delta time.
     * @param time the delta time
     */
    void onUpdate(double time) override;

    /**
     * Handles input events.
     * Does nothing.
     * @param event the input event
     */
    void onInputEvent(Event &event) override;

    /**
     * Fills this component with the information in the specified xml node.
     * @param node the xml node
     */
    void fill(const XMLNode &node) override;

    /**
     * Completes initialization. This method is called after all children are initialized. It is used to extract information for children.
     * Calls the onInit method of the native script.
     */
    void postFill() override;
private:
    std::unique_ptr<NativeScript> script;
    std::string scriptKey;
};

}


