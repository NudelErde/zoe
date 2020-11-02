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
    /**
     * Constructs a KeyEvent with the keycode that triggered this event.
     * @param keycode the specified keycode
     */
    KeyEvent(int keycode) : keyCode(keycode) {};

    /**
     * Destructs the KeyEvent.
     */
    virtual ~KeyEvent() {}

    /**
     * The keycode that triggered the event.
     */
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
     * Returns the number of repetitions.
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
};

/**
 * An event which indicates that a char is produced by the operating system text input system.
 * CharInputEvent obeys keyboard layouts and modifier keys and supports composing characters using dead keys
 */
class CharInputEvent : public EventTemplate<EventType::CharInput, EventCategory::Keyboard, EventCategory::Input> {
public:
    /**
     * Constructs a CharInputEvent with the specified codepoint.
     */
    CharInputEvent(unsigned int codepoint) : codepoint(codepoint) {}

    /**
     * Returns the codepoint produced by the operating system.
     * @return the codepoint produced by the operating system
     */
    [[nodiscard]] inline const unsigned int& getCodePoint() const noexcept { return codepoint; }
private:
    unsigned int codepoint;
};

}
