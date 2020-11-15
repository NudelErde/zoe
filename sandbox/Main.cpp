//
// Created by florian on 19.03.20.
//

#include "zoe.h"

using namespace Zoe;

class ButtonScript: public NativeScript {
public:
    int number = 0;

    void onActivation() override {
        infof("Test {}", number++);
        Scheduler::addTask([]() -> Task{
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
        uiLayer->setCamera(std::make_shared<Camera2D>(vec2({0, 0}), 1600.0f, 900.0f));
        uiLayer->load(File("sampleObjects/UITest.xml"));
        auto button1 = uiLayer->getChildByIDAndType<Button>("button1");
        button1->setClickHandler([=]() {
            button1->setPosition(button1->getPosition()+vec3(1,1,0));
        });

        getLayerStack().pushLayer(uiLayer);
    }

    ~App() override = default;
};

Application *Zoe::createApplication() {
    return new App();
}
