//
// Created by Florian on 24.11.2020.
//

#pragma once

#include "../Component.h"
#include "AxisAlignedBox.h"
#include "vector"
#include "algorithm"

namespace Zoe {

class PhysicsGroup;

class PhysicsComponent : public BaseComponent {
public:

    [[nodiscard]] inline const AxisAlignedBox& getAxisAlignedBox() const { return advancedAlignedBox; }

    virtual void onCollision(PhysicsComponent& other, double delta, const std::function<void()>& resolve) = 0;

    virtual void updateBoundingBox() = 0;

    std::function<void()> addAcceleration(const vec3& acceleration);

    std::function<void()> addForce(const vec3& force);

    void removeAcceleration(const std::function<void()>& remover);

    void removeForce(const std::function<void()>& remover);

    void addImpulse(const vec3& impulse);

    [[nodiscard]] inline const vec3& getVelocity() const { return velocity; }
    inline void setVelocity(const vec3& vel) { velocity = vel; }

    inline void setMass(const float& m) { mass = m; }
    [[nodiscard]] inline const float& getMass() const { return mass; }

    inline void setPhysicsGroupName(const std::string& name) { physicsGroupName = name; }
    [[nodiscard]] inline const std::string& getPhysicsGroupName() const { return physicsGroupName; }

protected:
    /**
     * Updates this component. This function is called as fast as possible. It shouldn't take long to execute this function.
     * @param delta the time in seconds since the last update
     */
    void onPhysicsUpdate(double delta) final;

    /**
     * Updates this component.
     * @param time the time in seconds since the last update
     */
    void onUpdate(double time) final;

protected:
    AxisAlignedBox alignedBox;
    AxisAlignedBox advancedAlignedBox;

    [[nodiscard]] inline const vec3& getLastPosition() { return lastPosition; }

private:
    friend class PhysicsGroup;

    vec3 lastPosition;
    vec3 velocity;
    float mass;
    std::vector<std::pair<vec3, int>> accelerations;
    int nextAccelerationID;
    std::string physicsGroupName;
    bool hasInit{};
    std::weak_ptr<PhysicsGroup> group;
};

}
