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
    /**
     * The title of the window.
     */
	std::string title;

	/**
	 * The width of the window in pixels.
	 */
	unsigned int width;

	/**
	 * The height of the window in pixels.
	 */
	unsigned int height;

	/**
	 * Creates a window with the specified arguments.
	 * @param title the title
	 * @param width the width in pixel
	 * @param height the height in pixel
	 */
	explicit WindowProps(const std::string& title = "Zoe Engine", unsigned int width =
			1280, unsigned int height = 720) :
			title(title), width(width), height(height) {
	}
};

/**
 * An abstract Window. A window is created by Application on program start.
 */
class DLL_PUBLIC Window {
public:
    /**
     * The callback type.
     */
	using EventCallbackFn = std::function<void(Event&)>;

	/**
	 * Destructs the window.
	 */
	virtual ~Window() = default;

    /**
     * Pull events and swap buffers
     */
	virtual void onUpdate() = 0;

    /**
     * return the windows width
     * @return width
     */
	virtual unsigned int getWidth() const = 0;

    /**
     * return the windows height
     * @return height
     */
	virtual unsigned int getHeight() const = 0;

	virtual void setSize(unsigned int width, unsigned int height) = 0;
	virtual void setTitle(const std::string& str) = 0;
	virtual std::string getTitle() = 0;

    /**
     * Sets the event callback function. The callback function is used when events are available.
     * @param callback the callback function
     */
	virtual void setEventCallback(const EventCallbackFn& callback) = 0;

    /**
     * Set vSync.
     * @param enabled true if vSync should be enabled
     */
	virtual void setVSync(bool enabled) = 0;

    /**
     * Returns true if vSync is enabled.
     * @returns true if vSync is enabled
     */
	virtual bool isVSync() const = 0;

    /**
     * Get native window handler.
     * @returns native window handler
     */
	virtual void* getNativeWindow() const = 0;

    /**
     * Returns used graphics context
     * @return graphics context
     */
	virtual GraphicsContext* getContext() const = 0;

	/**
	 * Creates a new window instance with the specified properties.
	 * @param props the specified properties
	 * @returns a window instance
	 */
	static Window* create(const WindowProps& props = WindowProps());

	/**
	 * Releases all window resources and unloads the library.
	 */
	static void shutdown();
};

}

