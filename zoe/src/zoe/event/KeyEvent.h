/*
 * KeyEvent.h
 *
 *  Created on: 04.04.2019
 *      Author: florian
 */

#pragma once

#include "Event.h"
#include "../core/Core.h"

namespace Zoe {

/**
 * An event which indicates that a key was pressed or released.
 * Can be a KeyPressedEvent or a KeyReleasedEvent.
 * @tparam type the specific EventType
 */
template<EventType type>
class DLL_PUBLIC KeyEvent : public EventTemplate<type, EventCategory::Keyboard, EventCategory::Input> {
public:
    /**
     * Returns the key code of the button that triggered the event.
     * @returns the key code of the button that triggered the event
     */
    inline int getKeyCode() const { return keyCode; }

protected:
    KeyEvent(int keycode) : keyCode(keycode) {};
    virtual ~KeyEvent() {}

    int keyCode;
};

/**
 * An event which indicates that a key was pressed.
 */
class DLL_PUBLIC KeyPressedEvent : public KeyEvent<EventType::KeyPressed> {
public:

    /**
     * Constructs a KeyPressedEvent with the specified keycode.
     * @param keycode the pressed keycode
     * @param repeatcount the number of repetitions
     */
    KeyPressedEvent(int keycode, int repeatcount) : KeyEvent(keycode), repeatCount(repeatcount) {}

    /**
     * Destructs the KeyPressedEvent.
     */
    ~KeyPressedEvent() {}

    /**
     * Returns the number of repetitions
     * @return the number of repetitions
     */
    inline int getRepeatCount() const { return repeatCount; }

private:
    int repeatCount;
};

/**
 * An event which indicates that a key was released.
 */
class DLL_PUBLIC KeyReleasedEvent : public KeyEvent<EventType::KeyReleased> {
public:
    /**
     * Constructs a KeyReleasedEvent with the specified keycode.
     * @param keycode the released keycode
     */
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    /**
     * Destructs the KeyReleasedEvent.
     */
    ~KeyReleasedEvent() {}

};

}
