//
// Created by Florian on 24.11.2020.
//

#include "PhysicsComponent.h"
#include "../ComponentLayer.h"
#include "PhysicsGroup.h"

void Zoe::PhysicsComponent::onUpdate(double time) {
    if (!hasInit && !physicsGroupName.empty()) {
        if(auto lay = getLayer(); lay != nullptr) {
            auto ptr = lay->getChildByIDAndType<PhysicsGroup>(physicsGroupName);
            if (ptr != nullptr) { //init only when group was found
                hasInit = true;
                group = ptr;
                ptr->addPhysicsObject(std::dynamic_pointer_cast<PhysicsComponent>(shared_from_this()));
            }
        }
    }
}
void Zoe::PhysicsComponent::onPhysicsUpdate(double delta) {
    lastPosition = getWorldPosition();
    vec3 acceleration;
    for (const auto& p : accelerations) {
        acceleration = acceleration + p.first;
    }
    vec3 pos = getPosition() + (velocity * (float) delta) + (acceleration / 2) * (float) delta * (float) delta;
    setPosition(pos);
    velocity = velocity + acceleration * (float)delta;
}
void Zoe::PhysicsComponent::addImpulse(const Zoe::vec3& impulse) {
    velocity = velocity + (impulse / mass);
}
std::function<void()> Zoe::PhysicsComponent::addForce(const Zoe::vec3& force) {
    return addAcceleration(force / mass);
}
std::function<void()> Zoe::PhysicsComponent::addAcceleration(const Zoe::vec3& acceleration) {
    int myID = nextAccelerationID++;
    accelerations.emplace_back(acceleration, myID);
    return [myID, weak = weak_from_this()]() {
        if (auto ptr = std::dynamic_pointer_cast<PhysicsComponent>(weak.lock())) {
            auto& acc = ptr->accelerations;
            acc.erase(std::remove_if(acc.begin(), acc.end(),
                                     [&](const std::pair<vec3, int>& p) { return p.second == myID; }), acc.end());
        }
    };
}
void Zoe::PhysicsComponent::removeAcceleration(const std::function<void()>& remover) {
    remover();
}
void Zoe::PhysicsComponent::removeForce(const std::function<void()>& remover) {
    remover();
}
