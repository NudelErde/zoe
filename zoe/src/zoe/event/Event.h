/*
 * Event.h
 *
 *  Created on: 04.04.2019
 *      Author: florian
 */

#pragma once

#include "../Core.h"
#include <functional>
#include <iostream>

namespace Zoe {

enum class EventType {
	EventNone = 0,
	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	WindowMoved,
	AppTick,
	AppUpdate,
	AppRender,
	KeyPressed,
	KeyReleased,
	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseScrolled
};

enum EventCategory {
	EventCategoryNone = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryWindow = BIT(1),
	EventCategoryInput = BIT(2),
	EventCategoryKeyboard = BIT(3),
	EventCategoryMouse = BIT(4),
	EventCategoryMouseButton = BIT(5)
};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type ; }\
	virtual EventType getEventType() const override {return getStaticType(); }\
	virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override {return category; }

class DLL_PUBLIC Event {
	friend class EventDispatcher;
public:
	virtual ~Event() {
	}
	virtual EventType getEventType() const = 0;
	virtual const char* getName() const = 0;
	virtual int getCategoryFlags() const = 0;
	virtual std::string toString() const {
		return getName();
	}

	inline bool isHandled() const {
		return handled;
	}

	inline bool isInCategory(EventCategory category) {
		return getCategoryFlags() & category;
	}
protected:
	bool handled = false;
};

class DLL_PUBLIC EventDispatcher {
	template<typename T>
	using EventFn = std::function<bool(T&)>;

public:
	EventDispatcher(Event& e) :
			event(e) {
	}

	template<typename T>
	bool dispatch(EventFn<T> func) {
		if (event.getEventType() == T::getStaticType()) {
			event.handled = func(*(T*) &event);
			return true;
		}
		return false;
	}

private:
	Event& event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
	return os << e.getName();
}

}
