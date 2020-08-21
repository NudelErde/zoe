//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"

class App : public Zoe::Application {
public:
    App() {
        std::shared_ptr<Zoe::ComponentLayer> ptr = std::make_shared<Zoe::ComponentLayer>();
        ptr->load(Zoe::File("C:/dev/vs/zoe/sandbox/sampleObjects/Cube.xml"));
        std::shared_ptr<Zoe::Camera3D> cam = std::make_shared<Zoe::Camera3D>(Zoe::vec3({0, 0, 0}), Zoe::vec3({0, 0, 0}),
                                                                             90.0f, 16.0f / 9.0f);
        ptr->setCamera(cam);
        getLayerStack().pushLayer(ptr);
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
