/*
 * application.cpp
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

#include "Application.h"

#include "../event/ApplicationEvent.h"

#include "Console.h"
#include "KeyCode.h"
#include "../render/Font.h"

#include "../display/Component.h"
#include "../display/UI/UITool.h"
#include "VirtualFiles.h"
#include "Task.h"
#include "Scheduler.h"

namespace Zoe {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

static Application* s_Instance;
static std::shared_ptr<Render> displayRender;

Application::Application(bool withWindow) : hasWindow(withWindow) {
    if (s_Instance != nullptr) {
        throw std::runtime_error("Application already exists!");
    }
    s_Instance = this;
    Console::init();
    //-------------------
    if (withWindow) {

        window = std::unique_ptr<Zoe::Window>(Zoe::Window::create());
        std::function<void(Event&)> cb = BIND_EVENT_FN(onEvent);
        window->setEventCallback(cb);
    } else {
        Scheduler::exit();
    }
    //---Init other stuff
    initVirtualFiles();
    initKeyMap();
    FontHolder::init();
    BaseComponent::init();
    if (withWindow) {
        UITool::init();
        displayRender = getContext().getRender();
        displayRender->setAlphaEnabled(true);
        displayRender->setRenderTarget(getContext().getDefaultRenderTarget());
    }
}

Application::~Application() {
    Window::shutdown();
}

void Application::onEvent(Event& e) {

    EventDispatcher dispatcher(e);
    { //---WindowClose-------------
        std::function<bool(WindowCloseEvent&)> cb = BIND_EVENT_FN(
                onWindowClose);
        dispatcher.dispatch<WindowCloseEvent>(cb);
    }
    layerStack.dispatchEvent(e);
}

Task Application::renderWindow() {
    while (true) {
        co_yield true;
        window->onUpdate(); //onUpdate swaps buffers. Do not clear before buffers swap! Current image would be cleared
        displayRender->clear();
        AppRenderEvent event;
        layerStack.dispatchEvent(event);
    }
}


Task Application::updateObjects() {
    while (true) {
        co_yield true;
        AppUpdateEvent event;
        layerStack.dispatchEvent(event);
    }
}
Task Application::tickObjects() {
    while (true) {
        co_yield true;
        AppTickEvent event;
        layerStack.dispatchEvent(event);
    }
}

void Application::run() {
    Scheduler::addTask(renderWindow());
    Scheduler::addTask(updateObjects());
    Scheduler::addTask(tickObjects());
}

void Application::exit() {
    Scheduler::exit();
}

bool Application::onWindowClose(WindowCloseEvent& e) {
    Scheduler::exit();
    return false;
}
Application& Application::get() {
    return *s_Instance;
}
}

int main() {
    try {
        auto app = Zoe::createApplication();
        app->run(); //setup tasks
        Zoe::Scheduler::execute();
        delete app;
        exit(EXIT_SUCCESS);
    } catch (const std::exception& e) {
        Zoe::critical("Uncaught error: ", e.what());
        exit(EXIT_FAILURE);
    }

}