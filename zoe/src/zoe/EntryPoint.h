/*
 * EntryPoint.h
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

#pragma once

#include "Application.h"
#include <cerrno>

extern Zoe::Application* Zoe::createApplication();

int main(){
    try{
        auto app = Zoe::createApplication();
        app->run();
        delete app;
        return EXIT_SUCCESS;
    }catch(const std::exception& e){
        Zoe::critical("Uncaught error: ", e.what());
        std::cin.get();
        return EXIT_FAILURE;
    }
}
