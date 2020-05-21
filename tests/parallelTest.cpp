//
// Created by florian on 08.03.20.
//

#include <zoe.h>
#include <EntryPoint.h>

using namespace Zoe;

class App: public Application{
public:
    App(): Application(false){
        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        PARALLEL(100, i, {
            sleep(10);
        });
        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
        unsigned int time1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        start = std::chrono::steady_clock::now();
        for(unsigned int i = 0; i < 100; ++i){
            sleep(10);
        }
        end = std::chrono::steady_clock::now();
        unsigned int time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        info("info1: ",time1);
        info("info2: ",time2);
        if(time1 > time2)
            throw std::runtime_error("Test Failed");
        this->exit();
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
