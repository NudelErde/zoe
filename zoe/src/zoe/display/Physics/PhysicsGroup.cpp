//
// Created by Florian on 23.11.2020.
//

#include "PhysicsGroup.h"
#include <string>
#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include "BoxCollider.h"

namespace Zoe {

void PhysicsGroup::onDraw(const Camera& camera) {

}
void PhysicsGroup::onUpdate(double time) {

}
void PhysicsGroup::onInputEvent(Event& event) {

}
void PhysicsGroup::fill(const XMLNode& node) {
    if (auto iter = node.attributes.find("maxDirectChecks"); iter != node.attributes.end()) {
        maxDirectChecks = std::stoi(iter->second);
    }
    if (auto iter = node.attributes.find("recursionLimit"); iter != node.attributes.end()) {
        recursionLimit = std::stoi(iter->second);
    }
}
void PhysicsGroup::postFill() {

}
void PhysicsGroup::onPhysicsUpdate(double delta) {
    checkCollision(delta);
}
void PhysicsGroup::addPhysicsObject(const std::shared_ptr<PhysicsComponent>& physicsComponent) {
    PhysicsCheckObject pco;
    pco.wComp = physicsComponent;
    elements.push_back(pco);
}
PhysicsGroup::PhysicsGroup() = default;

//----------------------------------------------------------------------------------------------------------------------
//Specific type checks
//----------------------------------------------------------------------------------------------------------------------

bool specificCheckBoxBox(const std::shared_ptr<PhysicsComponent>& a_abs,
                         const std::shared_ptr<PhysicsComponent>& b_abs,
                         double delta) {
    auto a = std::dynamic_pointer_cast<BoxCollider>(a_abs);
    auto b = std::dynamic_pointer_cast<BoxCollider>(b_abs);
    std::function < void() > nothing = []() {};

    if (a->collisionBehavior == BoxCollider::CollisionBehavior::AUTO &&
        b->collisionBehavior == BoxCollider::CollisionBehavior::STATIC) {
        std::swap(a, b);
    }

    if (a->collisionBehavior == BoxCollider::CollisionBehavior::STATIC &&
        b->collisionBehavior == BoxCollider::CollisionBehavior::AUTO) {
        AxisAlignedBox sBox(a->alignedBox.getHigherCorner() + (b->dimension / 2),
                            a->alignedBox.getLowerCorner() - (b->dimension / 2));
        vec3 center = b->alignedBox.getCenter();
        vec3 lastCenter = b->getLastPosition() + b->dimension / 2;
        if (center == lastCenter) {
            if (a->alignedBox.checkCollision(b->alignedBox)) {
                a->onCollision(*b, delta, nothing);
                b->onCollision(*a, delta, nothing);
                return true;
            }
            return false;
        }
        vec3 direction = center - lastCenter;

        float timeXMin = (sBox.getLowerCorner().x - lastCenter.x) / direction.x;
        float timeXMax = (sBox.getHigherCorner().x - lastCenter.x) / direction.x;
        float timeYMin = (sBox.getLowerCorner().y - lastCenter.y) / direction.y;
        float timeYMax = (sBox.getHigherCorner().y - lastCenter.y) / direction.y;
        float timeZMin = (sBox.getLowerCorner().z - lastCenter.z) / direction.z;
        float timeZMax = (sBox.getHigherCorner().z - lastCenter.z) / direction.z;

        if (timeXMin > timeXMax) {
            std::swap(timeXMin, timeXMax);
        }
        if (timeYMin > timeYMax) {
            std::swap(timeYMin, timeYMax);
        }
        if (timeZMin > timeZMax) {
            std::swap(timeZMin, timeZMax);
        }
        float timeMin = std::max({timeXMin, timeYMin, timeZMin});
        float timeMax = std::min({timeXMax, timeYMax, timeZMax});
        if ((timeMin < timeMax && timeMin <= 1 && timeMin >= 0) ||
            a->alignedBox.checkCollision(b->alignedBox)) { //check if b is in a
            //Collision
            std::function < void() > bResolve = [=]() {
                b->position = b->getLastPosition() + timeMin * direction;
                ///@todo make velocity stuff thing
            };
            a->onCollision(*b, delta, nothing);
            b->onCollision(*a, delta, bResolve);
            return true;
        }
        return false;

    } else if (a->collisionBehavior == BoxCollider::CollisionBehavior::AUTO &&
               b->collisionBehavior == BoxCollider::CollisionBehavior::AUTO) {
        ///@todo magic
        //magic condition
        //  resolve functions and onCollision
        //  return true
        debug("Fuck u!");

        return false;
    } else {
        if (a->alignedBox.checkCollision(b->alignedBox)) {
            a->onCollision(*b, delta, nothing);
            b->onCollision(*a, delta, nothing);
            return true;
        }
        return false;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//Double dynamic type resolution
//----------------------------------------------------------------------------------------------------------------------

void PhysicsGroup::init() {
    PhysicsGroup::addSpecificCollisionCheck<BoxCollider, BoxCollider>(specificCheckBoxBox);
}

void PhysicsGroup::addSpecificCollisionCheck(const std::type_index& typeA, const std::type_index& typeB,
                                             const std::function<bool(const std::shared_ptr<PhysicsComponent>&,
                                                                      const std::shared_ptr<PhysicsComponent>&,
                                                                      double)>& function) {
    specificCollisionCheckMap[typeB][typeA] = [function](const std::shared_ptr<PhysicsComponent>& a,
                                                         const std::shared_ptr<PhysicsComponent>& b,
                                                         double delta) -> bool {
        return function(b, a, delta);
    };
    specificCollisionCheckMap[typeA][typeB] = function;
}

void PhysicsGroup::checkSpecificCollision(const std::shared_ptr<PhysicsComponent>& a,
                                          const std::shared_ptr<PhysicsComponent>& b,
                                          double delta) {
    auto func = specificCollisionCheckMap[std::type_index(typeid(*a))][std::type_index(typeid(*b))];
    if (func) {
        func(a, b, delta);
    }
}

//----------------------------------------------------------------------------------------------------------------------
//Collision check
//----------------------------------------------------------------------------------------------------------------------

void PhysicsGroup::checkCollision(double delta) {
    std::vector<CollisionCheck> checks;
    octCheck(elements, lastGlobalCenter, recursionLimit, 0, checks);
    std::sort(checks.begin(), checks.end(), [](const CollisionCheck& a, const CollisionCheck& b) {
        return a.distance < b.distance; //smallest distance first
    });
    for (auto& check : checks) {
        checkSpecificCollision(check.a, check.b, delta);
    }
    for (auto& element : elements) {
        element.sComp = nullptr; // remove shared pointer to allow deletion of components
    }
}

void PhysicsGroup::octCheck(std::vector<PhysicsCheckObject>& vector, const vec3& center,
                            uint8_t recursionCheckVariable, uint8_t prevFlag,
                            std::vector<CollisionCheck>& collisionChecks) const {
    if(vector.empty())
        return;
    if (recursionCheckVariable == 0) [[unlikely]] { //fallback
        for (size_t i = 0; i < vector.size() - 1; ++i) {
            for (size_t j = i + 1; j < vector.size(); ++j) {
                vector[i].sComp = vector[i].wComp.lock();
                vector[j].sComp = vector[j].wComp.lock();
                if (vector[i].sComp != nullptr && vector[j].sComp != nullptr) {
                    vector[i].sComp->updateBoundingBox();
                    vector[j].sComp->updateBoundingBox();
                    if(vector[i].sComp->advancedAlignedBox.checkCollision(vector[j].sComp->advancedAlignedBox)) {
                        vec3 diff = vector[i].sComp->advancedAlignedBox.getCenter() -
                                    vector[j].sComp->advancedAlignedBox.getCenter();
                        collisionChecks.emplace_back(vector[i].sComp, vector[j].sComp,
                                                     diff * diff);
                    }
                }
            }
        }
        return;
    }

    size_t directionCount[8]{};
    //pXpYpZ - +X+Y+Z - 0
    //pXpYnZ - +X+Y-Z - 1
    //pXnYpZ - +X-Y+Z - 2
    //pXnYnZ - +X-Y-Z - 3
    //nXpYpZ - -X+Y+Z - 4
    //nXpYnZ - -X+Y-Z - 5
    //nXnYpZ - -X-Y+Z - 6
    //nXnYnZ - -X-Y-Z - 7

    //check in which octant the object is (oct-tree would be nice but objects can change position without a physics update)
    for (auto& element : vector) {
        if (recursionLimit == recursionCheckVariable) {
            //check if first recursion //Branch Predictor and Compiler should optimize this check significantly
            element.sComp = element.wComp.lock(); //create shared_ptr when in first recursion
            if (element.sComp == nullptr) {
                //ignore invalid elements (invalid elements can only be in the first recursion)
                continue;
            }
            element.sComp->updateBoundingBox(); //update bounding box in first recursion
        }

        //check in which octant the element is and increment it's counter
        //one object can be in more than one octant
        const AxisAlignedBox& box = element.sComp->getAxisAlignedBox();
        const vec3& high = box.getHigherCorner();
        const vec3& low = box.getLowerCorner();
        bool posX = high.x >= center.x;
        bool negX = low.x <= center.x;
        bool posY = high.y >= center.y;
        bool negY = low.y <= center.y;
        bool posZ = high.z >= center.z;
        bool negZ = low.z <= center.z;

        element.positionFlag.pXpYpZ = posX && posY && posZ;
        element.positionFlag.pXpYnZ = posX && posY && negZ;
        element.positionFlag.pXnYpZ = posX && negY && posZ;
        element.positionFlag.pXnYnZ = posX && negY && negZ;
        element.positionFlag.nXpYpZ = negX && posY && posZ;
        element.positionFlag.nXpYnZ = negX && posY && negZ;
        element.positionFlag.nXnYpZ = negX && negY && posZ;
        element.positionFlag.nXnYnZ = negX && negY && negZ;

        if (element.positionFlag.pXpYpZ) ++directionCount[0];
        if (element.positionFlag.pXpYnZ) ++directionCount[1];
        if (element.positionFlag.pXnYpZ) ++directionCount[2];
        if (element.positionFlag.pXnYnZ) ++directionCount[3];
        if (element.positionFlag.nXpYpZ) ++directionCount[4];
        if (element.positionFlag.nXpYnZ) ++directionCount[5];
        if (element.positionFlag.nXnYpZ) ++directionCount[6];
        if (element.positionFlag.nXnYnZ) ++directionCount[7];
    }

    std::vector<PhysicsCheckObject> directionArray[8]{
            std::vector<PhysicsCheckObject>(directionCount[0]),
            std::vector<PhysicsCheckObject>(directionCount[1]),
            std::vector<PhysicsCheckObject>(directionCount[2]),
            std::vector<PhysicsCheckObject>(directionCount[3]),
            std::vector<PhysicsCheckObject>(directionCount[4]),
            std::vector<PhysicsCheckObject>(directionCount[5]),
            std::vector<PhysicsCheckObject>(directionCount[6]),
            std::vector<PhysicsCheckObject>(directionCount[7])
    };
    size_t directionIndex[8]{};

    vec3 directionAverage[8]{};


    for (auto& element : vector) {
        if (element.sComp == nullptr)
            continue;
        auto& myAligned = element.sComp->advancedAlignedBox;
        vec3 myCenter = myAligned.getCenter();
        uint8_t checkedDirections = 0; // these 8 bits fill from least to most significant bit
        for (uint8_t direction = 0; direction < 8; ++direction) { // check `element` for all 8 directions
            if (element.positionFlag.flag & (1u << direction)) { // `element` is in this direction
                if (directionCount[direction] > 1 && directionCount[direction] <= maxDirectChecks) {
                    for (size_t i = 0; i < directionIndex[direction]; ++i) {
                        auto& otherElement = directionArray[direction][i];
                        if ((recursionCheckVariable == recursionLimit ||
                             !((otherElement.prevRecFlag.flag & element.prevRecFlag.flag) & prevFlag))
                            /*first check or not checked in parallel check*/

                            && !((otherElement.positionFlag.flag & element.positionFlag.flag) & checkedDirections)) {
                            // not already checked in different direction
                            // `checkedDirections` has a bit activated for every checked direction
                            // If `otherElement` is in a direction in which `element` is also located and which is also
                            // checked, then these two elements have already been checked for collisions
                            if(element.sComp->advancedAlignedBox.checkCollision(otherElement.sComp->advancedAlignedBox)) {
                                auto& otherAligned = otherElement.sComp->advancedAlignedBox;
                                vec3 connectionVector = myCenter - otherAligned.getCenter();
                                collisionChecks.emplace_back(element.sComp, otherElement.sComp,
                                                             connectionVector * connectionVector);
                            }
                        }
                    }
                }
                //collect elements for this direction
                //in the case that directionCount <= maxDirectChecks => every combination only once
                //otherwise collect elements for next recursion
                directionArray[direction][directionIndex[direction]++] = element;
                element.prevRecFlag = element.positionFlag;
                directionAverage[direction] = directionAverage[direction]
                                              + element.sComp->advancedAlignedBox.getHigherCorner()
                                              + element.sComp->advancedAlignedBox.getLowerCorner();
            }
            checkedDirections <<= 1u;
            checkedDirections |= 1u;
        }
    }

    uint8_t checkedDirections = 0;
    for (uint8_t direction = 0; direction < 8; ++direction) {
        //optimisation: parallel? => NO because parallel makes weird stuff
        if (directionCount[direction] > maxDirectChecks) {
            octCheck(directionArray[direction], directionAverage[direction] / (float) (directionCount[direction] * 2),
                     recursionCheckVariable - 1, checkedDirections, collisionChecks);
        }
        checkedDirections <<= 1u;
        checkedDirections |= 1u;
    }
}

}
