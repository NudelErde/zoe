//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"
#include <cmath>

class App : public Zoe::Application {
public:
    App() {
        auto layer = std::make_shared<Zoe::ComponentLayer>();
        layer->setCamera(Zoe::Camera2D(Zoe::vec2({0, 0}), 1600, 900));
        auto button = std::make_shared<Zoe::Button>();
        layer->add(button);
        button->setPosition(Zoe::vec3({0,0,0}));
        button->setSize(Zoe::vec2({100,100}));
        button->setClickHandler([](){Zoe::info("Test");});
        getLayerStack().pushLayer(layer);
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
