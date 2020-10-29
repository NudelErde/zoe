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

namespace Zoe {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

static Application* s_Instance;
static std::shared_ptr<Render> displayRender;

Application::Application(bool withWindow): hasWindow(withWindow) {
	if(s_Instance!=nullptr){
		throw std::runtime_error("Application already exists!");
	}
	s_Instance = this;
    Console::init();
    //-------------------
    if(withWindow) {

        window = std::unique_ptr<Zoe::Window>(Zoe::Window::create());
        std::function<void(Event&)> cb = BIND_EVENT_FN(onEvent);
        window->setEventCallback(cb);
    }else{
        running = false;
    }
    //---Init other stuff
    initVirtualFiles();
    initKeyMap();
    FontHolder::init();
    BaseComponent::init();
    if(withWindow){
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

void Application::run() {
	while (this->running) {
		window->onUpdate();//onUpdate swaps buffers do not clear before buffers swap => current image is cleared
        displayRender->clear();
        { //Dispatch AppEvents
			AppTickEvent event;
			layerStack.dispatchEvent(event);
		}
		{
			AppUpdateEvent event;
			layerStack.dispatchEvent(event);
		}
		{
			AppRenderEvent event;
			layerStack.dispatchEvent(event);
		}
	}
	debug("Close application");
}

void Application::exit() {
	this->running = false;
}

bool Application::onWindowClose(WindowCloseEvent& e) {
	this->running = false;
	return false;
}

Application &Application::get() {
    return *s_Instance;
}

}
