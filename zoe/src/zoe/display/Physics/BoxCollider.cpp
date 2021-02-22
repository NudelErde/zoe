//
// Created by Florian on 26.12.2020.
//

#include "BoxCollider.h"

namespace Zoe {

void BoxCollider::onDraw(const Camera& camera) {

}
void BoxCollider::onInputEvent(Event& event) {

}
void BoxCollider::fill(const XMLNode& node) {
    if (auto iter = node.attributes.find("dx"); iter != node.attributes.end()) {
        dimension.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("dy"); iter != node.attributes.end()) {
        dimension.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("dz"); iter != node.attributes.end()) {
        dimension.z = std::stof(iter->second);
    }

    if (auto iter = node.attributes.find("auto"); iter != node.attributes.end()) {
        if (iter->second == "auto" || iter->second == "move") {
            collisionBehavior = CollisionBehavior::AUTO;
        } else if (iter->second == "static") {
            collisionBehavior = CollisionBehavior::STATIC;
        }
    }

    if (auto iter = node.attributes.find("group"); iter != node.attributes.end()) {
        setPhysicsGroupName(iter->second);
    }

    vec3 vel;
    if (auto iter = node.attributes.find("vel_x"); iter != node.attributes.end()) {
        vel.x = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("vel_y"); iter != node.attributes.end()) {
        vel.y = std::stof(iter->second);
    }
    if (auto iter = node.attributes.find("vel_z"); iter != node.attributes.end()) {
        vel.z = std::stof(iter->second);
    }
    setVelocity(vel);

    float mass = 1;
    if (auto iter = node.attributes.find("mass"); iter != node.attributes.end()) {
        mass = std::stof(iter->second);
    }
    setMass(mass);

    if (auto iter = node.attributes.find("script"); iter != node.attributes.end()) {
        scriptComponentID = iter->second;
    }
}
void BoxCollider::postFill() {
    if (!scriptComponentID.empty()) {
        script = getChildByIDAndType<ScriptComponent>(scriptComponentID);
    }
}
void BoxCollider::onCollision(PhysicsComponent& other, double delta, const std::function<void()>& resolve) {
    if(auto ptr = script.lock()) {
        ptr->onCollision(delta, resolve);
    }
    if (collisionBehavior == CollisionBehavior::AUTO) {
        resolve();
    }
}
void BoxCollider::updateBoundingBox() {
    vec3 pos = getWorldPosition();
    alignedBox = AxisAlignedBox(pos, pos + dimension);
    advancedAlignedBox = AxisAlignedBox(std::min(pos.x, getLastPosition().x),
                                        std::min(pos.y, getLastPosition().y),
                                        std::min(pos.z, getLastPosition().z),
                                        std::max(pos.x + dimension.x, getLastPosition().x + dimension.x),
                                        std::max(pos.y + dimension.y, getLastPosition().y + dimension.y),
                                        std::max(pos.z + dimension.z, getLastPosition().z + dimension.z));
}

}
