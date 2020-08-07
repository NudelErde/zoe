/*
 * LinuxInput.h
 *
 *  Created on: 16.04.2019
 *      Author: florian
 */

#ifndef WIN32

#pragma once

#include "../../zoe/core/Input.h"

namespace Zoe {

class LinuxInput: public Input {
public:
	~LinuxInput();

protected:

	bool isKeyPressedImpl(int keycode) override;

	bool isMouseButtonPressedImpl(int button) override;
	vec2 getMousePositionImpl() override;
	float getMouseXImpl() override;
	float getMouseYImpl() override;
};

}

#endif