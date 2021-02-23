//
// Created by Florian on 29.08.2020.
//

#pragma once

#include "../core/Core.h"
#include "ScriptComponent.h"
#include <functional>

namespace Zoe {

class NativeScriptComponent;

/**
 * The native script. Inherit this class to create a c++ script.
 */
class DLL_PUBLIC NativeScript {
public:
    /**
     * The onUpdate function is called every game tick with the time since the last event.
     */
    virtual void onUpdate(const double&) {}

    /**
     * The onInit function is called once.
     * It is guaranteed that the onInit function is called before the onUpdate function is called the first time.
     */
    virtual void onInit() {}

    /**
     * The onActivation function is called when the parent component is activated.
     */
    virtual void onActivation() {}

    /**
     * The onCollision function is the collision handler of a PhysicsComponent.
     * @param delta the time since the last check
     * @param resolve the function that can resolve the collision
     */
    virtual void onCollision(double delta, const std::function<void()>& resolve) {}

    /**
     * Returns a shared pointer to the parent of the NativeScriptComponent.
     * @return the shared pointer
     */
    [[nodiscard]] inline std::shared_ptr<BaseComponent> getComponent() const { return component.lock(); }

    /**
     * The parent of the NativeScriptComponent.
     */
    std::weak_ptr<BaseComponent> component{};

    /**
     * The NativeScriptComponent.
     */
    std::weak_ptr<NativeScriptComponent> scriptComponent{};
};

/**
 * A native script is used to script in c++.
 */
class DLL_PUBLIC NativeScriptComponent : public ScriptComponent {
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
    static void
    registerNativeScript(const std::string& scriptName, const std::function<std::unique_ptr<NativeScript>()>& function);

    /**
     * Register a native script class with a specified name
     * @tparam ScriptClass the script class
     * @param scriptName the name of the script
     */
    template<typename ScriptClass>
    static inline void registerNativeScript(const std::string& scriptName) {
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
    static std::unique_ptr<NativeScript> getNativeScript(const std::string& scriptName);

    /**
     * Returns `true` if the specified script name exists.
     * @param scriptName the specified script name
     * @returns `true` if the script exists
     */
    static bool hasNativeScript(const std::string& scriptName);

    /**
     * The onActivation function is called when the parent component is activated.
     */
    void onActivation() override;

    /**
     * The onCollision function is the collision handler of a PhysicsComponent.
     * @param delta the time since the last check
     * @param resolve the function that can resolve the collision
     */
    void onCollision(double delta, const std::function<void()>& resolve) override;

public:
    /**
     * Creates an empty NativeScriptComponent.
     */
    NativeScriptComponent() = default;

    /**
     * Defines a default move constructor.
     */
    NativeScriptComponent(NativeScriptComponent&&) noexcept = default;

    /**
     * Defines a default move assign.
     * @return the result
     */
    NativeScriptComponent& operator=(NativeScriptComponent&&) noexcept = default;

    /**
     * Deletes the copy constructor.
     */
    NativeScriptComponent(const NativeScriptComponent&) = delete;

    /**
     * Deletes the copy assign.
     * @return the result
     */
    NativeScriptComponent& operator=(const NativeScriptComponent&) = delete;

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
    void onDraw(const Camera& camera) override;

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
    void onInputEvent(Event& event) override;

    /**
     * Fills this component with the information in the specified xml node.
     * @param node the xml node
     */
    void fill(const XMLNode& node) override;

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


