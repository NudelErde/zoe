/*
 * window.h
 *
 *  Created on: 08.04.2019
 *      Author: florian
 */

#pragma once

#include "../zpch.h"
#include "Core.h"
#include "../event/Event.h"
#include "../render/GraphicsContext.h"

namespace Zoe {

/**
 * Contains information about the window
 */
struct DLL_PUBLIC WindowProps {
	std::string title;
	unsigned int width;
	unsigned int height;

	/**
	 * Creates a window with the specified arguments.
	 * @param title the title
	 * @param width the width in pixel
	 * @param height the height in pixel
	 */
	WindowProps(const std::string& title = "Zoe Engine", unsigned int width =
			1280, unsigned int height = 720) :
			title(title), width(width), height(height) {
	}
};

class DLL_PUBLIC Window {
public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window() {
	}
	virtual void onUpdate() = 0;
	virtual unsigned int getWidth() const = 0;
	virtual unsigned int getHeight() const = 0;

	virtual void setEventCallback(const EventCallbackFn& callback) = 0;
	virtual void setVSync(bool enabled) = 0;
	virtual bool isVSync() const = 0;

	virtual void* getNativeWindow() const = 0;

	virtual GraphicsContext* getContext() const = 0;

	static Window* create(const WindowProps& props = WindowProps());
	static void shutdown();
};

}

