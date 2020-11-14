//
// Created by florian on 19.03.20.
//

#include "zoe.h"

using namespace Zoe;

class App : public Application {
public:
    App() {
        std::shared_ptr<ComponentLayer> uiLayer = std::make_shared<ComponentLayer>();
        uiLayer->setCamera(std::make_shared<Camera2D>(vec2({0, 0}), 1600.0f, 900.0f));
        uiLayer->load(File("sampleObjects/UITest.xml"));
        std::dynamic_pointer_cast<Button>(uiLayer->getChildren()[1])->setClickHandler([=]() {
            uiLayer->getChildren()[1]->setPosition(uiLayer->getChildren()[1]->getPosition()+vec3(1,1,0));
        });

        getLayerStack().pushLayer(uiLayer);
    }

    ~App() override = default;
};

Application *Zoe::createApplication() {
    return new App();
}
