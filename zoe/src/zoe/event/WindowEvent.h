/*
 * WindowEvent.h
 *
 *  Created on: 05.04.2019
 *      Author: florian
 */

#pragma once

#include "Event.h"
#include "../core/Core.h"

namespace Zoe {

/**
 * An event which indicates that the window was resized.
 */
class DLL_PUBLIC WindowResizeEvent : public EventTemplate<EventType::WindowResize, EventCategory::Window> {
public:
    /**
     * Constructs a WindowResizeEvent with the new window size.
     * @param width the new width
     * @param height the new height
     */
    WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}

    /**
     * Destructs the WindowResizeEvent.
     */
    ~WindowResizeEvent() {}

    /**
     * Returns the new height.
     * @returns the new height
     */
    inline unsigned int getHeight() const { return height; }

    /**
     * Returns the new width.
     * @return the new width
     */
    inline unsigned int getWidth() const { return width; }

private:
    unsigned int width, height;
};

/**
 * An event which indicates that the window was closed.
 */
class DLL_PUBLIC WindowCloseEvent : public EventTemplate<EventType::WindowClose, EventCategory::Window> {
public:

    /**
     * Constructs a WindowCloseEvent.
     */
    WindowCloseEvent() {}

    /**
     * Destructs the WindowCloseEvent.
     */
    ~WindowCloseEvent() {}
};

}
