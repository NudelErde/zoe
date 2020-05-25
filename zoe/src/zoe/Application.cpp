/*
 * application.cpp
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

#include "Application.h"

#include "event/ApplicationEvent.h"

#include "Console.h"
#include "KeyCode.h"

#include "game/nodes/2DGameNode.h"

namespace Zoe {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

static Application* s_Instance;

Application::Application(bool withWindow): hasWindow(withWindow) {
	if(s_Instance!=nullptr){
		critical("Application already exists!");
		Application::get().exit();
		return;
	}
	s_Instance = this;
	//---Init other stuff
	Console::init();
	Layer2D::init();
	initKeyMap();

	//-------------------
	if(withWindow) {
        window = std::unique_ptr<Zoe::Window>(Zoe::Window::create());
        std::function<void(Event&)> cb = BIND_EVENT_FN(onEvent);
        window->setEventCallback(cb);
    }else{
	    running = false;
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
		window->onUpdate();
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
