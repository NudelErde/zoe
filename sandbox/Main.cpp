//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"
#include <cmath>

class UITestLayer : public Zoe::UILayer{
public:

    void onAttach() override{
        setColor(Zoe::Color({1,1,0,1}));
        addEllipse(10,10,100,100);


        Zoe::Font font(Zoe::File("Product Sans Regular.ttf"), 80);

        //addImage(300,100,600,600,font.getBitmap());

        addText({100,100}, "UTF-8 test Lööps", Zoe::Color({1,0,0,1}), font);
    }

    void onDetach() override{

    }

};

class Layer : public Zoe::Layer3D {
public:

    void onAttach() override {
        Zoe::Material material(Zoe::File("shader.glsl"));
        Zoe::Model model(Zoe::File("Model.obj"));
        model.setModelMatrix(Zoe::scale3D(2,2,2));
        std::shared_ptr<Zoe::Camera> camera = std::make_shared<Zoe::Camera>();
        camera->setPosition({0, 7, 10});
        camera->setRotation(0, 0, 0);
        camera->setProjectionMatrix(Zoe::scale3D(4,4,1)*Zoe::perspective(0.05f, 100, 70, 16.0f / 9.0f));
        cube = std::make_shared<Zoe::Object3D>(material, model);
        add(cube);
        setCamera(camera);
    }

    void onDetach() override {

    }

    void onTick() override {

    }

    void onRender() override {
        static float position = 0;
        static float rotation = 0;
        cube->getMaterial().setUniform("lightSource", Zoe::vec4({10,10,std::sin(position)*10,1}));
        cube->getMaterial().setUniform("lightColor", Zoe::vec3({1,1,1}));
        cube->getMaterial().setUniform("ambiLight", Zoe::vec3({0.2f,0.2f,0.2f}));
        if(Zoe::Input::isKeyPressed(KEY_SPACE)){
            rotation += 0.1f;
            cube->getModel().setModelMatrix(Zoe::scale3D(2,2,2)*Zoe::rotateXZ3D(rotation));
        }
        if(Zoe::Input::isKeyPressed(KEY_ENTER)){
            position += 0.01f;
        }
    }
private:
    std::shared_ptr<Zoe::Object3D> cube;
};

class App : public Zoe::Application {
public:
    App() {
        getLayerStack().pushLayer(new Layer());
        getLayerStack().pushLayer(new UITestLayer());
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
