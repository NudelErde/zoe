/*
 * MouseEvent.h
 *
 *  Created on: 04.04.2019
 *      Author: florian
 */

#pragma once

#include "Event.h"
#include "../core/Core.h"

namespace Zoe{

class DLL_PUBLIC MouseMovedEvent: public Event{
public:
	MouseMovedEvent(float x,float y): mouseX(x),mouseY(y) {}
	~MouseMovedEvent(){}
	inline float getX() const { return mouseX; }
	inline float getY() const { return mouseY; }

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float mouseX,mouseY;
};

class DLL_PUBLIC MouseScrolledEvent : public Event{
public:
	MouseScrolledEvent(float x, float y): xOffset(x),yOffset(y) {}
	~MouseScrolledEvent(){}
	inline float getXOffset() const { return xOffset; }
	inline float getYOffset() const { return yOffset; }

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float xOffset,yOffset;
};

class DLL_PUBLIC MouseButtonEvent : public Event{
public:
	inline int getMouseButton() const { return button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)
protected:
	MouseButtonEvent(int button): button(button) {}
	virtual ~MouseButtonEvent(){}

	int button;
};

class DLL_PUBLIC MouseButtonPressedEvent : public MouseButtonEvent{
public:
	MouseButtonPressedEvent(int button): MouseButtonEvent(button) {}
	~MouseButtonPressedEvent(){}
	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class DLL_PUBLIC MouseButtonReleasedEvent : public MouseButtonEvent{
public:
	MouseButtonReleasedEvent(int button): MouseButtonEvent(button) {}
	~MouseButtonReleasedEvent(){}
	EVENT_CLASS_TYPE(MouseButtonReleased)
};

}
