/*
 * KeyEvent.h
 *
 *  Created on: 04.04.2019
 *      Author: florian
 */

#pragma once

#include "Event.h"
#include "../Core.h"

namespace Zoe{

class DLL_PUBLIC KeyEvent: public Event{
public:
	inline int getKeyCode() const {return keyCode;}

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
	KeyEvent(int keycode): keyCode(keycode){};
	virtual ~KeyEvent(){}

	int keyCode;
};

class DLL_PUBLIC KeyPressedEvent: public KeyEvent{
public:
	KeyPressedEvent(int keycode, int repeatcount): KeyEvent(keycode), repeatCount(repeatcount){}
	~KeyPressedEvent(){}

	inline int getRepeatCount() const {return repeatCount; }

	EVENT_CLASS_TYPE(KeyPressed)

private:
	int repeatCount;
};

class DLL_PUBLIC KeyReleasedEvent: public KeyEvent{
public:
	KeyReleasedEvent(int keycode): KeyEvent(keycode) {}
	~KeyReleasedEvent(){}

	EVENT_CLASS_TYPE(KeyReleased)
};

}
