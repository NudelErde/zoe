/*
 * MouseEvent.h
 *
 *  Created on: 04.04.2019
 *      Author: florian
 */

#pragma once

#include "Event.h"
#include "../core/Core.h"

namespace Zoe {

/**
 * An event which indicates that the mouse was moved.
 * The coordinates are relative to the top left corner of the window and range from 0 to 1.
 */
class DLL_PUBLIC MouseMovedEvent
        : public EventTemplate<EventType::MouseMoved, EventCategory::Mouse, EventCategory::Input> {
public:
    /**
     * Constructs a MouseMovedEvent with the specified coordinates.
     * @param x the x coordinate of the mouse
     * @param y the y coordinate of the mouse
     */
    MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}
    /**
     * Destructs the MouseMovedEvent.
     */
    ~MouseMovedEvent() {}
    /**
     * Returns the x position of the mouse.
     * @returns the x position of the mouse
     */
    inline float getX() const { return mouseX; }
    /**
     * Returns the y position of the mouse.
     * @returns the y position of the mouse
     */
    inline float getY() const { return mouseY; }

private:
    float mouseX, mouseY;
};

/**
 * An event which indicates that the mouse wheel was used.
 * X and Y coordinates indicate how the mouse wheel was used.
 */
class DLL_PUBLIC MouseScrolledEvent
        : public EventTemplate<EventType::MouseScrolled, EventCategory::Mouse, EventCategory::Input> {
public:
    /**
     * Constructs a MouseMovedEvent with the specified offset.
     * @param x the x offset of the mouse scroll event
     * @param y the y offset of the mouse scroll event
     */
    MouseScrolledEvent(float x, float y) : xOffset(x), yOffset(y) {}
    /**
     * Destructs the MouseScrolledEvent.
     */
    ~MouseScrolledEvent() {}
    /**
     * Returns how much the mouse was scrolled in the X-direction.
     * @returns how much the mouse was scrolled in the X-direction
     */
    inline float getXOffset() const { return xOffset; }
    /**
     * Returns how much the mouse was scrolled in the Y-direction.
     * @returns how much the mouse was scrolled in the Y-direction
     */
    inline float getYOffset() const { return yOffset; }

private:
    float xOffset, yOffset;
};

/**
 * An event which indicates that a mouse button was pressed or released.
 * Can be a MouseButtonPressedEvent or a MouseButtonReleasedEvent.
 * @tparam type the specific EventType
 */
template<EventType type>
class DLL_PUBLIC MouseButtonEvent : public EventTemplate<type, EventCategory::MouseButton, EventCategory::Input> {
public:
    /**
     * Returns the pressed button code.
     * @returns the pressed button code
     */
    inline int getMouseButton() const { return button; }

protected:
    MouseButtonEvent(int button) : button(button) {}
    virtual ~MouseButtonEvent() {}

    int button;
};

/**
 * An event which indicates that a mouse button was pressed.
 */
class DLL_PUBLIC MouseButtonPressedEvent : public MouseButtonEvent<EventType::MouseButtonPressed> {
public:
    /**
     * Constructs a MouseButtonPressedEvent with the specified button.
     * @param button the pressed button
     */
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
    /**
     * Destructs the MouseButtonPressedEvent.
     */
    ~MouseButtonPressedEvent() {}
};

/**
 * An event which indicates that a mouse button was released.
 */
class DLL_PUBLIC MouseButtonReleasedEvent : public MouseButtonEvent<EventType::MouseButtonReleased> {
public:
    /**
     * Constructs a MouseButtonReleasedEvent with the specified button.
     * @param button the released button
     */
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
    /**
     * Destructs the MouseButtonReleasedEvent.
     */
    ~MouseButtonReleasedEvent() {}
};

}
