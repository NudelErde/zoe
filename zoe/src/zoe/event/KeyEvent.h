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

template<EventType type>
class DLL_PUBLIC KeyEvent : public EventTemplate<type, EventCategory::Keyboard, EventCategory::Input> {
public:
    inline int getKeyCode() const { return keyCode; }

protected:
    KeyEvent(int keycode) : keyCode(keycode) {};
    virtual ~KeyEvent() {}

    int keyCode;
};

class DLL_PUBLIC KeyPressedEvent : public KeyEvent<EventType::KeyPressed> {
public:
    KeyPressedEvent(int keycode, int repeatcount) : KeyEvent(keycode), repeatCount(repeatcount) {}
    ~KeyPressedEvent() {}

    inline int getRepeatCount() const { return repeatCount; }

private:
    int repeatCount;
};

class DLL_PUBLIC KeyReleasedEvent : public KeyEvent<EventType::KeyReleased> {
public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}
    ~KeyReleasedEvent() {}

};

}
