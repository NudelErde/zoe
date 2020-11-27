/*
 * Event.h
 *
 *  Created on: 04.04.2019
 *      Author: florian
 */

#pragma once

#include "../core/Core.h"
#include <functional>
#include <iostream>

namespace Zoe {

/**
 * The type of Event instances.
 */
enum class EventType : unsigned int {
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
    MouseScrolled,
    CharInput
};

/**
 * The Category of Event instances. An Event can be in multiple Categories.
 */
enum class EventCategory : unsigned int {
    None = 0,
    Application = BIT(0),
    Window = BIT(1),
    Input = BIT(2),
    Keyboard = BIT(3),
    Mouse = BIT(4),
    MouseButton = BIT(5)
};

/**
 * An abstract Event.
 */
class Event {
    friend class EventDispatcher;

public:
    /**
     * Destructs this Event.
     */
    virtual ~Event() {}

    /**
     * Returns the event type of this Object.
     * @return the event type of this Object
     */
    virtual EventType getEventType() const = 0;
    /**
     * Returns the category flag.
     * @return the category flag
     */
    virtual unsigned int getCategoryFlags() const = 0;

    /**
     * Returns if this event was already handled. An handled event should be ignored.
     * @return `true` if this event was handled.
     */
    inline bool isHandled() const {
        return handled;
    }

    /**
     * Returns if this event is in the specified EventCategory.
     * @param category the specified EventCategory
     * @return `true` if this event is in the category
     */
    inline bool isInCategory(EventCategory category) {
        return getCategoryFlags() & (unsigned int) category;
    }
protected:
    /**
     * `handled` is true if the event is handled and should be ignored.
     */
    bool handled = false;
};

/**
 * The EventDispatcher is used to send events to the right handling function.
 * ```cpp
 * static function<void(KeyPressedEvent&)> keyPressFunction = &bar0;
 * static function<void(MouseScrolledEvent&)> mouseScrollFunction = &bar1;
 *
 * void foo(Event& e) {
 *     EventDispatcher dispatcher(e);
 *     dispatcher.dispatch(keyPressFunction);
 *     dispatcher.dispatch(mouseScrollFunction);
 * }
 *
 * void bar0(KeyPressedEvent& keyEvent);
 * void bar1(MouseScrolledEvent& keyEvent);
 * ```
 */
class EventDispatcher {
    template<typename T>
    using EventFn = std::function<void(T&)>;

public:
    /**
     * Create an EventDispatcher for a specified event.
     * @param e the specified object
     */
    EventDispatcher(Event& e) :
            event(e) {
    }

    /**
     * Calls the handling function when the Event used to create this dispatcher is accepted by the function `func`.
     * @tparam T the type that the handling function accepts
     * @param func the handling function
     * @returns `true` if the event handling function was called
     */
    template<typename T>
    bool dispatch(EventFn<T> func) {
        if (event.getEventType() == T::getStaticType()) {
            func(*(T*) &event);
            return true;
        }
        return false;
    }

private:
    Event& event;
};

/**
 * Template for Event implementation
 * @tparam type the event type
 * @tparam category a list of event categories
 */
template<EventType type, EventCategory... category>
class EventTemplate : public Event {
public:

    /**
     * Returns the category flag.
     * @return the category flag
     */
    unsigned int getCategoryFlags() const override {
        return (((unsigned int) category) | ...);
    }

    /**
     * Returns the event type of this Object.
     * @return the event type of this Object
     */
    EventType getEventType() const override {
        return type;
    }

    /**
     * Returns the event type of this Class.
     * @return the event type of this Class
     */
    static EventType getStaticType() {
        return type;
    }
};

}
