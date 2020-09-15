/*
 * WindowsInput.h
 *
 *  Created on: 16.04.2019
 *      Author: florian
 */

#ifdef WIN32

#pragma once

#include "../../zoe/zpch.h"
#include "../../zoe/core/Input.h"

namespace Zoe {

class WindowsInput: public Input {
public:
	~WindowsInput();

protected:

	bool isKeyPressedImpl(int keycode) override;

	bool isMouseButtonPressedImpl(int button) override;
	vec2 getMousePositionImpl() override;
	float getMouseXImpl() override;
	float getMouseYImpl() override;
};

}

#endif