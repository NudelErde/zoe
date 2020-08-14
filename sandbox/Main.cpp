//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"
#include <cmath>

class App : public Zoe::Application {
public:
    App() {
        Zoe::MaterialLibrary::parseMaterialLibrary(Zoe::File("sampleObjects/Cube/Cube.mtl"));
        Zoe::MaterialLibrary::parseMaterialLibrary(Zoe::File("sampleObjects/Cube/Cube.mtl"));
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
