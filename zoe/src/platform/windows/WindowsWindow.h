/*
 * WindowsWindow.h
 *
 *  Created on: 08.04.2019
 *      Author: florian
 */

#if defined(WIN32) || defined(DOXYGEN)

#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "../../zoe/core/Window.h"
#include "../../zoe/core/Core.h"

namespace Zoe{

/**
 * WindowsWindow is the implementation for Window on Windows.
 */
class DLL_PUBLIC WindowsWindow: public Window{
public:
    /**
     * Creates WindowsWindow with the specified WindowProps
     * @param props the properties used to create this window
     */
    explicit WindowsWindow(const WindowProps& props);
    /**
     * Destructs the window and releases used resources
     */
    ~WindowsWindow() override;

    /**
     * Pull events and swap buffers
     */
    void onUpdate() override;

    /**
     * return the windows width
     * @return width
     */
    inline unsigned int getWidth() const override {return data.width;}
    /**
     * return the windows height
     * @return height
     */
    inline unsigned int getHeight() const override {return data.height;}

    /**
     * Sets the event callback function. The callback function is used when events are available.
     * @param callback the callback function
     */
    inline void setEventCallback(const EventCallbackFn& callback) override {data.eventCallback = callback;}
    /**
     * Set vSync.
     * @param enabled true if vSync should be enabled
     */
    void setVSync(bool enabled) override;
    /**
     * Returns true if vSync is enabled.
     * @returns true if vSync is enabled
     */
    bool isVSync() const override;

    /**
     * Get native window handler.
     * @returns native window handler
     */
    inline void* getNativeWindow() const override {return window;}

    /**
     * Returns used graphics context
     * @return graphics context
     */
    inline GraphicsContext* getContext() const override {return context;}

    void setSize(unsigned int width, unsigned int height) override;
    void setTitle(const std::string& str) override;
    std::string getTitle() override;
private:
	void init(const WindowProps& props);
	void shutdown();

	void setUpListener();
private:
	GLFWwindow* window;

	GraphicsContext* context;

	struct WindowData{
		std::string title;
		unsigned int width,height;
		bool vSync;

		EventCallbackFn eventCallback;
	};

	WindowData data;
};

}

#endif