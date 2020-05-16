//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"

class Layer : public Zoe::Layer3D {
public:

    void onAttach() override {
        /*Zoe::Material material(Zoe::File("shader.glsl"));
        Zoe::Model model(Zoe::File("CubeNoSub.obj"));
        std::shared_ptr<Zoe::Camera> camera = std::make_shared<Zoe::Camera>();
        camera->setPosition({0, 0, 6});
        camera->setRotation(0, 0, 0);
        camera->setProjectionMatrix(Zoe::perspective(0.1, 100, 90, 16.0 / 9.0));
        add(Zoe::Object3D(material, model));
        setCamera(camera);*/
    }

    void onDetach() override {

    }

    void onTick() override {

    }

    void onRender() override {

    }

};

class App : public Zoe::Application {
public:
    App() {
        getLayerStack().pushLayer(new Layer());
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
