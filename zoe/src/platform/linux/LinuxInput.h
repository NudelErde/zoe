/*
 * LinuxInput.h
 *
 *  Created on: 16.04.2019
 *      Author: florian
 */

#pragma once

#include "../../zoe/zpch.h"
#include "../../zoe/Input.h"

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
