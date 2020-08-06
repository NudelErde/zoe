//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"
#include <cmath>

class App : public Zoe::Application {
public:
    App() {
        auto UILayer = std::make_shared<Zoe::ComponentLayer>();
        auto modelLayer = std::make_shared<Zoe::ComponentLayer>();
        auto modelCamera = std::make_shared<Zoe::Camera3D>(Zoe::vec3({0,3,-4}), Zoe::vec3({0,0,0}), 70.0f, 16.0f/9.0f);

        UILayer->setCamera(std::make_shared<Zoe::Camera2D>(Zoe::vec2({0, 0}), 1600.0f, 900.0f));
        auto button = std::make_shared<Zoe::Button>();
        button->setPosition(Zoe::vec3({0,0,0}));
        button->setSize(Zoe::vec2({100,100}));
        button->setClickHandler([this](){this->exit();});
        button->setColor(Zoe::vec4({1,0,0,1}));
        UILayer->add(button);

        modelLayer->setCamera(modelCamera);
        auto worldObject = std::make_shared<Zoe::WorldObject>();
        worldObject->setModel(Zoe::Model(Zoe::File("Model.obj")));
        modelLayer->add(worldObject);


        getLayerStack().pushLayer(modelLayer);
        getLayerStack().pushLayer(UILayer);
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
