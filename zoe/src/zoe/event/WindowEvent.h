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

class DLL_PUBLIC WindowResizeEvent : public EventTemplate<EventType::WindowResize, EventCategory::Window> {
public:
    WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}
    ~WindowResizeEvent() {}

    inline unsigned int getHeight() const { return height; }
    inline unsigned int getWidth() const { return width; }

private:
    unsigned int width, height;
};

class DLL_PUBLIC WindowCloseEvent : public EventTemplate<EventType::WindowClose, EventCategory::Window> {
public:
    WindowCloseEvent() {}
    ~WindowCloseEvent() {}
};

}
