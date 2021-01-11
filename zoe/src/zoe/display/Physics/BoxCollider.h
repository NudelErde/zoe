//
// Created by Florian on 26.12.2020.
//

#pragma once

#include "PhysicsComponent.h"
#include "../ScriptComponent.h"

namespace Zoe {

class BoxCollider : public PhysicsComponent {
public:

    void onCollision(PhysicsComponent& other, double delta, const std::function<void()>& resolve) override;

    void updateBoundingBox() override;

protected:
    /**
     * Draws this component on the specified camera.
     * @param camera the specified camera
     */
    void onDraw(const Camera& camera) override;
    /**
     * Handles input events.
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
     */
    void postFill() override;

private:
    vec3 dimension;
    enum class CollisionBehavior {
        OFF, STATIC, AUTO
        //OFF -> Do nothing
        //STATIC -> Do not move
        //AUTO -> Move
    };

    CollisionBehavior collisionBehavior = CollisionBehavior::OFF;
    std::weak_ptr<ScriptComponent> script{};
    std::string scriptComponentID;

    friend bool specificCheckBoxBox(const std::shared_ptr<PhysicsComponent>& a_abs,
                                    const std::shared_ptr<PhysicsComponent>& b_abs,
                                    double delta);
};


}
