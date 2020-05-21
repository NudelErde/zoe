/*
 * application.h
 *
 *  Created on: 31.03.2019
 *      Author: florian
 */

#pragma once

#include "Core.h"
#include "Console.h"
#include "Window.h"
#include "LayerStack.h"
#include "render/GraphicsContext.h"

namespace Zoe {

class DLL_PUBLIC Application {
public:
	Application(bool withWindow = true);
	virtual ~Application();

	void run();
	void onEvent(Event& e);

	inline LayerStack& getLayerStack() {
		return layerStack;
	}

	void exit();

	inline Window& getWindow(){
		return *window;
	}

	static Application& get();

	inline static GraphicsContext& getContext(){
		return *(get().getWindow().getContext());
	}
private:
	bool onWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> window;
	bool running = true;

	LayerStack layerStack;
	bool hasWindow;
};

Application* createApplication();

}
