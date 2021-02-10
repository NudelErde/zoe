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

/**
 * A PhysicsComponent is the base component for all components with physical interaction.
 * Each PhysicsComponent is connected to a PhysicsGroup. Only Components in the same PhysicsGroup can collide.
 */
class PhysicsComponent : public BaseComponent {
public:

    /**
     * Returns the internal AxisAlignedBox. This value is updated by calling updateBoundingBox()
     * @return the internal AxisAlignedBox
     */
    [[nodiscard]] inline const AxisAlignedBox& getAxisAlignedBox() const { return advancedAlignedBox; }

    /**
     * onCollision is called when a collision has occurred with this object.
     * @param other the other object
     * @param delta the time since the last physics update
     * @param resolve a function to resolve the collision
     */
    virtual void onCollision(PhysicsComponent& other, double delta, const std::function<void()>& resolve) = 0;

    /**
     * updateBoundingBox updates the internal bounding box.
     */
    virtual void updateBoundingBox() = 0;

    /**
     * Adds an acceleration by the specified vector. This acceleration can be removed by calling the returned object or
     * passing the object in the removeAcceleration() function.
     * @param acceleration the specified acceleration vector
     * @return the remover object
     */
    std::function<void()> addAcceleration(const vec3& acceleration);

    /**
     * Adds a force by the specified vector. This force can be removed by calling the returned object or
     * passing the object in the removeForce() function.
     * @param force the specified force vector
     * @return the remover object
     */
    std::function<void()> addForce(const vec3& force);

    /**
     * Remove the acceleration specified by the parameter.
     * @param remover the acceleration remover object
     */
    void removeAcceleration(const std::function<void()>& remover);

    /**
     * Remove the force specified by the parameter.
     * @param remover the force remover object
     */
    void removeForce(const std::function<void()>& remover);

    /**
     * Adds an impulse specified by the impulse vector.
     * @param impulse the impulse vector
     */
    void addImpulse(const vec3& impulse);

    /**
     * Returns the current velocity.
     * @return the current velocity
     */
    [[nodiscard]] inline const vec3& getVelocity() const { return velocity; }

    /**
     * Sets the current velocity to the specified value.
     * @param vel the specified velocity
     */
    inline void setVelocity(const vec3& vel) { velocity = vel; }

    /**
     * Sets the current mass to the specified value.
     * @param m the specified mass
     */
    inline void setMass(const float& m) { mass = m; }

    /**
     * Returns the current mass.
     * @return the current mass
     */
    [[nodiscard]] inline const float& getMass() const { return mass; }

    /**
     * Sets the connected PhysicsGroup name.
     * @param name the PhysicsGroup name
     */
    inline void setPhysicsGroupName(const std::string& name) { physicsGroupName = name; }

    /**
     * Returns the connected PhysicsGroup name.
     * @return the PhysicsGroup name
     */
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
    AxisAlignedBox alignedBox; ///< The box bounding the object.
    AxisAlignedBox advancedAlignedBox; ///< The box bounding the object at its current and last position.

    /**
     * Returns the position before the current position.
     * @return the last position
     */
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
