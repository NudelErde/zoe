//
// Created by Florian on 23.11.2020.
//

#pragma once

#include "../Component.h"
#include "PhysicsComponent.h"
#include <string>
#include <cstddef>
#include <concepts>
#include <functional>
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>

namespace Zoe {

//remove invalid elements on game tick not physics tick
//create new lastGlobalCenter on game tick

/**
 * A PhysicsGroup connects multiple PhysicsComponents. Only the PhysicsComponents in the same group can collide.
 */
class PhysicsGroup : public BaseComponent {
public:
    PhysicsGroup(); ///< Creates an empty PhysicsGroup.

    /**
     * Check the collision for a specified delta time.
     * @param delta the delta time
     */
    void checkCollision(double delta);

    /**
     * Add a PhysicsComponent to this group
     * @param physicsComponent the specified component
     */
    void addPhysicsObject(const std::shared_ptr<PhysicsComponent>& physicsComponent);

    /**
     * Initializes the specific checks.
     */
    static void init();

protected:
    /**
     * Draws this component on the specified camera.
     * @param camera the specified camera
     */
    void onDraw(const Camera& camera) override;
    /**
     * Updates this component.
     * @param time the time in seconds since the last update
     */
    void onUpdate(double time) override;
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
    /**
     * Updates this component. This function is called as fast as possible. It shouldn't take long to execute this function.
     * @param delta the time in seconds since the last update
     */
    void onPhysicsUpdate(double delta) override;

    /**
     * Checks for a collision between a and b in the time delta. A specific check is used.
     * @param a the first object
     * @param b the second object
     * @param delta the delta time
     */
    static void checkSpecificCollision(const std::shared_ptr<PhysicsComponent>& a,
                                       const std::shared_ptr<PhysicsComponent>& b,
                                       double delta);

    /**
     * Adds a specific collision check for TypeA and TypeB.
     * @tparam TypeA the first type
     * @tparam TypeB the second type
     * @param function the specific collision check
     */
    template<typename TypeA, typename TypeB>
    static void addSpecificCollisionCheck(const std::function<bool(const std::shared_ptr<PhysicsComponent>&,
                                                                   const std::shared_ptr<PhysicsComponent>&, double)>& function) {
        addSpecificCollisionCheck(std::type_index(typeid(TypeA)), std::type_index(typeid(TypeB)), function);
    }

    /**
     * Adds a specific collision check for typeA and typeB.
     * @param typeA the first type
     * @param typeB the second type
     * @param function the specific collision check
     */
    static void addSpecificCollisionCheck(const std::type_index& typeA, const std::type_index& typeB,
                                          const std::function<bool(const std::shared_ptr<PhysicsComponent>&,
                                                                   const std::shared_ptr<PhysicsComponent>&, double)>& function);

private:
    inline static std::map<std::type_index, std::map<std::type_index,
            std::function<bool(const std::shared_ptr<PhysicsComponent>&,
                               const std::shared_ptr<PhysicsComponent>&, double)>>> specificCollisionCheckMap;

private:
    struct PhysicsCheckObject {
        union PositionFlag {
            struct {
                bool pXpYpZ: 1;
                bool pXpYnZ: 1;
                bool pXnYpZ: 1;
                bool pXnYnZ: 1;
                bool nXpYpZ: 1;
                bool nXpYnZ: 1;
                bool nXnYpZ: 1;
                bool nXnYnZ: 1;
            };
            uint8_t flag; ///< a bitflag representation of the PositionFlag
        };///< The position flag is used to store in which octant the Object is

        std::weak_ptr<PhysicsComponent> wComp;
        std::shared_ptr<PhysicsComponent> sComp;
        PositionFlag positionFlag;
        PositionFlag prevRecFlag;
    };

    struct CollisionCheck {
        std::shared_ptr<PhysicsComponent> a;
        std::shared_ptr<PhysicsComponent> b;
        double distance;
    };

    void octCheck(std::vector<PhysicsCheckObject>& vector, const vec3& center, uint8_t recursionCheckVariable,
                  uint8_t prevFlag, std::vector<CollisionCheck>& collisionChecks) const;

    std::vector<PhysicsCheckObject> elements;

    vec3 lastGlobalCenter{};
    size_t maxDirectChecks = 128;
    uint8_t recursionLimit = 16;
};

}
