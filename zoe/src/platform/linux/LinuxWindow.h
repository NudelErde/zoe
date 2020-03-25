/*
 * LinuxWindow.h
 *
 *  Created on: 08.04.2019
 *      Author: florian
 */

#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "../../zoe/Window.h"
#include "../../zoe/Core.h"

namespace Zoe{

class DLL_PUBLIC LinuxWindow: public Window{
public:

	LinuxWindow(const WindowProps& props);
	~LinuxWindow();

	void onUpdate() override;

	inline unsigned int getWidth() const override {return data.width;}
	inline unsigned int getHeight() const override {return data.height;}

	inline void setEventCallback(const EventCallbackFn& callback) override {data.eventCallback = callback;}
	void setVSync(bool enabled) override;
	bool isVSync() const override;

	inline void* getNativeWindow() const override {return window;}

	inline GraphicsContext* getContext() const override {return context;}
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
