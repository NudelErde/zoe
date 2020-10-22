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
#include "../render/GraphicsContext.h"
#include "../render/Font.h"

namespace Zoe {

/**
 * Main Class\n
 * For the entry point, the developer needs to provide an implementation of the application class via Zoe::createApplication.\n
 * Only one instance can exist in the program.
 */
class DLL_PUBLIC Application {
public:
    /**
     * Creates the Application and initializes the library. \n
     * `withWindow` can be set to false to start zoe without a graphics context. This should be used in tests. \n
     * Parts that use the graphics context will not load and will likely crash the program if they are used.
     * @param withWindow
     */
	explicit Application(bool withWindow = true);
	/**
	 * Destructs the Application. Should be one of the last objects that will be destructed.
	 */
	virtual ~Application();

	/**
	 * Main loop. Will loop until the program is terminated with Application::exit or the window is closed.
	 */
	void run();
	/**
	 * Main event handler. Will dispatch events to layers if they subscribe to that EventCategory
	 * @param e event
	 */
	void onEvent(Event& e);

	/**
	 * provides access to LayerStack. Layers should be added here.
	 * @returns reference to the application's LayerStack
	 */
	inline LayerStack& getLayerStack() {
		return layerStack;
	}

	/**
	 * Exits clean. The program will be stopped at next loop iteration. Use std::exit to exit immediately.
	 */
	void exit();

	/**
	 * Returns Window. Is nullptr if application was create without window creation
	 * @returns window
	 */
	inline Window& getWindow(){
		return *window;
	}

	/**
	 * Returns Application instance.
	 * @returns Application instance
	 */
	static Application& get();

	/**
	 * Shortcut for `Application::get().getWindow().getContext()`
	 * @returns active graphics context
	 */
	inline static GraphicsContext& getContext(){
		return *(get().getWindow().getContext());
	}

	/**
	 * Returns the default Font.
	 * @returns the default Font
	 */
	static const Font& getDefaultFont();

private:
	bool onWindowClose(WindowCloseEvent& e);

	std::unique_ptr<Window> window;
	bool running = true;

	LayerStack layerStack;
	bool hasWindow;
};

/**
 * Needs to be provided by the developer. Is called once by the entry point.
 * @returns an instance of anything inheriting Application (your application)
 */
Application* createApplication();

}