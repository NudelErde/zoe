//
// Created by Florian on 28.12.2020.
//

#pragma once

#include "Component.h"

namespace Zoe {

class PhysicsComponent;

/**
 * ScriptComponent is the abstract script. Implemented by ChaiScriptComponent and NativeScriptComponent
 */
class ScriptComponent : public BaseComponent{
public:
    /**
     * The onActivation function is called when the parent component is activated.
     */
    virtual void onActivation() = 0;

    /**
     * The onCollision function is the collision handler of a PhysicsComponent.
     * @param delta the time since the last check
     * @param resolve the function that can resolve the collision
     */
    virtual void onCollision(double delta, const std::function<void()>& resolve, PhysicsComponent& other) = 0;
};

}
