/*
 * application.cpp
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

#include "Application.h"

#include "event/ApplicationEvent.h"

#include "Console.h"

namespace Zoe {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Instance = nullptr;

Application::Application() {
	if(s_Instance!=nullptr){
		Log::getLogger()->critical("Application already exists!");
		Application::get().exit();
		return;
	}
	s_Instance = this;
	//---Init other stuff
	Log::init();

	//-------------------
	window = std::unique_ptr<Zoe::Window>(Zoe::Window::create());
	std::function<void(Event&)> cb = BIND_EVENT_FN(onEvent);
	window->setEventCallback(cb);
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

}
