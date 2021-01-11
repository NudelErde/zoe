//
// Created by florian on 17.08.20.
//

#include <zoe.h>

using namespace Zoe;

class App: public Application{
public:
    App(): Application(false){
        Path p = Path("virtual/tests/test1.txt");
        File f1 = p;
        auto stream1ABase = f1.createIOStream();
        auto& stream1A = *stream1ABase;
        stream1A << "Test" << std::flush;
        auto stream1BBase = f1.createIStream();
        auto& stream1B = *stream1BBase;
        std::string str;
        stream1B>>str;
        if(str != "Test"){
            error("Could not read written value");
            debug(str);
            std::exit(-1);
        }
        Scheduler::addTask([](){
            Scheduler::exit();
        });
    }

    ~App() override = default;
};

Zoe::Application *Zoe::createApplication() {
    return new App();
}
