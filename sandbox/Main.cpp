//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include <type_traits>

using namespace Zoe;

class ButtonScript : public NativeScript {
public:
    int number = 0;

    void onActivation() override {
        infof("Test {}", number++);
        Scheduler::addTask([]() -> Task {
            using namespace std::chrono_literals;
            co_await 10s;
            info("Waited 10 seconds");
            co_await 1h;
            info("Long time");
        }());
    }
};

class App : public Application {
public:
    App() {
        NativeScriptComponent::registerNativeScript<ButtonScript>("ButtonScript");

        std::shared_ptr<ComponentLayer> uiLayer = std::make_shared<ComponentLayer>();
        uiLayer->load(File("sampleObjects/UITest.xml"));
        auto button1 = uiLayer->getChildByIDAndType<Button>("button1");
        button1->setClickHandler([uiLayer]() {
            auto button1 = uiLayer->getChildByIDAndType<Button>("button1");
            button1->setPosition(button1->getPosition() + vec3(1, 1, 0));
        });

        getLayerStack().pushLayer(uiLayer);
    }

    ~App() override = default;
};

Application* Zoe::createApplication() {
    return new App();
}
