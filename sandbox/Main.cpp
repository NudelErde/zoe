//
// Created by florian on 19.03.20.
//

#include "zoe.h"

using namespace Zoe;

class CameraScript : public NativeScript {
public:

    void onUpdate(const double& t) override {
        if (auto comp = std::dynamic_pointer_cast<Camera3D>(component.lock())) {
            vec3 pos = comp->getPosition();
            vec3 dir = comp->getDirection();
            vec3 norm = dir.crossProduct(vec3(0, 1, 0)).normalize();
            vec3 rot = comp->getRotation();
            if (Input::isKeyPressed(KEY_W)) {
                pos = pos + dir * (float) t;
            }
            if (Input::isKeyPressed(KEY_S)) {
                pos = pos + dir * (float) -t;
            }
            if (Input::isKeyPressed(KEY_A)) {
                pos = pos + norm * (float) t;
            }
            if (Input::isKeyPressed(KEY_D)) {
                pos = pos + norm * (float) -t;
            }
            if (Input::isKeyPressed(KEY_SPACE)) {
                pos = pos + vec3(0, 1, 0) * (float) t;
            }
            if (Input::isKeyPressed(KEY_LEFT_SHIFT)) {
                pos = pos + vec3(0, 1, 0) * (float) -t;
            }
            if (Input::isKeyPressed(KEY_LEFT)) {
                rot.y += (float) t;
            }
            if (Input::isKeyPressed(KEY_RIGHT)) {
                rot.y += (float) -t;
            }
            comp->setPosition(pos);
            comp->setRotation(rot);
            if (Input::isKeyPressed(KEY_ENTER)) {
                auto cube = comp->getLayer()->getChildByIDAndType<PhysicsComponent>("test");
                cube->setVelocity(vec3(0.5, 1, 0));
            }
            if (Input::isKeyPressed(KEY_BACKSPACE)) {
                auto cube = comp->getLayer()->getChildByIDAndType<PhysicsComponent>("test");
                cube->setVelocity(vec3(0, -1, -0.5));
            }
            if (Input::isKeyPressed(KEY_ESCAPE)) {
                auto cube = comp->getLayer()->getChildByIDAndType<PhysicsComponent>("test");
                cube->setVelocity(vec3());
                cube->setPosition(vec3(0,0.1,0));
            }
            if (Input::isKeyPressed(KEY_G)) {
                auto cube = comp->getLayer()->getChildByIDAndType<PhysicsComponent>("test");
                auto gravity = cube->addAcceleration(vec3(0, -0.1, 0));
                Scheduler::addCoroutine([gravity]() -> Task {
                    using namespace std::chrono_literals;
                    co_await 5s;
                    gravity();
                });
            }
        }
    }
};

class App : public Application {
public:
    App() {
        NativeScriptComponent::registerNativeScript<CameraScript>("CameraScript");
        std::shared_ptr<ComponentLayer> layer = std::make_shared<ComponentLayer>();

        layer->load(File("sampleObjects/PhysicsExample.xml"));
        //layer->load(File("sampleObjects/UITest.xml"));

        getLayerStack().pushLayer(layer);
    }

    ~App() override = default;
};

Application* Zoe::createApplication() {
    return new App();
}
