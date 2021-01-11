//
// Created by Florian on 21.12.2020.
//

#include <zoe.h>
#include <ctime>
#include <random>

using namespace Zoe;

class SimplePhysicsComponent : public PhysicsComponent {
public:

    explicit SimplePhysicsComponent(AxisAlignedBox box)
            : PhysicsComponent(vec3(), 1, "") {
        this->advancedAlignedBox = box;
    }

    void onCollision(PhysicsComponent& other, double delta) override {
        collided = true;
    }

    void updateBoundingBox() override {}

    bool collided = false;
};

class App : public Application {
public:
    App() : Application(false) {
        Scheduler::addTask([]() {
            Scheduler::exit();
        });

        {
            std::shared_ptr<PhysicsGroup> group = std::make_shared<PhysicsGroup>();
            std::shared_ptr<SimplePhysicsComponent> simp1 = std::make_shared<SimplePhysicsComponent>(
                    AxisAlignedBox(0, 0, 0, 10, 10, 10));
            std::shared_ptr<SimplePhysicsComponent> simp2 = std::make_shared<SimplePhysicsComponent>(
                    AxisAlignedBox(9, 9, 9, 11, 11, 11));
            std::shared_ptr<SimplePhysicsComponent> simp3 = std::make_shared<SimplePhysicsComponent>(
                    AxisAlignedBox(12, 12, 12, 15, 15, 15));

            group->addPhysicsObject(simp1);
            group->addPhysicsObject(simp2);
            group->addPhysicsObject(simp3);

            group->checkCollision(0);

            if (simp1->collided && simp2->collided && !simp3->collided) {
                info("Simple case done");
            } else {
                info("Simple case failed");
                std::exit(-1);
            }
        }
        {
            std::shared_ptr<PhysicsGroup> group = std::make_shared<PhysicsGroup>();
            auto seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 generator(seed);
            infof("Seed: {}", seed);
            std::uniform_int_distribution<int> distribution(-10'000, 10'000);
            constexpr unsigned int amount = 1000;
            std::vector<std::shared_ptr<SimplePhysicsComponent>> simps(amount);
            for (unsigned int i = 0; i < amount; ++i) {
                float x = distribution(generator);
                float y = distribution(generator);
                float z = distribution(generator);
                AxisAlignedBox AABB(x, y, z, x + 10, y + 10, z + 10);
                simps[i] = std::make_shared<SimplePhysicsComponent>(AABB);
                group->addPhysicsObject(simps[i]);
            }
            info("Start random check");
            auto start = std::chrono::steady_clock::now();
            group->checkCollision(0);
            infof("Time for check: {}ms", (std::chrono::duration_cast<std::chrono::nanoseconds>(
                    std::chrono::steady_clock::now() - start).count() / 1000000.0));
        }
        {
            info("Empty collision check");
            std::shared_ptr<PhysicsGroup> group = std::make_shared<PhysicsGroup>();
            group->checkCollision(0);
        }
    }

    ~App() override = default;
};

Zoe::Application* Zoe::createApplication() {
    return new App();
}
