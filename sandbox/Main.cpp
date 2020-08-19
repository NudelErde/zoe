//
// Created by florian on 19.03.20.
//

#include "zoe.h"
#include "EntryPoint.h"

class App : public Zoe::Application {
public:
    App() {
        Zoe::WavefrontFile::parseWavefrontFile(Zoe::File("C:/dev/vs/zoe/sandbox/sampleObjects/Cube/Cube.obj"));
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
