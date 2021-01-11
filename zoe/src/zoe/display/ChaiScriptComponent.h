//
// Created by Florian on 29.08.2020.
//

#pragma once

#include "../core/Core.h"
#include "ScriptComponent.h"

namespace Zoe {

struct ChaiScriptInstance;

/**
 * A chai script is used to script in chaiscript.
 * @see https://chaiscript.com/
 */
class DLL_PUBLIC ChaiScriptComponent : public ScriptComponent {
public:
    /**
     * Creates an empty ChaiScriptComponent.
     */
    ChaiScriptComponent();

    /**
     * Defines a default move constructor.
     */
    ChaiScriptComponent(ChaiScriptComponent&&) noexcept;

    /**
     * Defines a default move assign.
     * @return the result
     */
    ChaiScriptComponent& operator=(ChaiScriptComponent&&) noexcept;

    /**
     * Deletes the copy constructor.
     */
    ChaiScriptComponent(const ChaiScriptComponent&) = delete;

    /**
     * Deletes the copy assign.
     * @return the result
     */
    ChaiScriptComponent& operator=(const ChaiScriptComponent&) = delete;

    /**
     * Destructs the ChaiScriptComponent.
     */
    ~ChaiScriptComponent();
protected:

    /**
     * Draws this component on the specified camera.
     * Does nothing.
     * @param camera the specified camera
     */
    void onDraw(const Camera& camera) override;

    /**
     * Calls the onUpdate methode of the script with the delta time.
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
     * Calls the onInit method of the script.
     */
    void postFill() override;

    /**
     * The onActivation function is called when the parent component is activated.
     */
    void onActivation() override;

    /**
     * The onCollision function is the collision handler of a PhysicsComponent.
     * @param delta the time since the last check
     */
    void onCollision(double delta, const std::function<void()>& resolve) override;
private:
    std::unique_ptr<ChaiScriptInstance> script;
};

}