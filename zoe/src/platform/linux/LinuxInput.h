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

/**
 * LinuxInput is the implementation for Input on Linux based system.
 */
class LinuxInput: public Input {
public:
    /**
     * destructs LinuxInput
     */
	~LinuxInput();

protected:

    /**
     * Check if a specific keycode is pressed. KeyCodes are defined in the KeyCode.h file. Keycode constants begin with KEY_
     * @param keycode is the checked keycode
     * @returns true if key is pressed
     */
	bool isKeyPressedImpl(int keycode) override;

	/**
	 * Check if a specific mouse button is pressed. MouseCodes are defined in the MouseCode.h file. MouseCode constants begin with MOUSE_
	 * @param button is the checked mouse button
	 * @returns true if mouse is pressed
	 */
	bool isMouseButtonPressedImpl(int button) override;

	/**
	 * Get the mouse position in the window. The values range from 0 to 1.
	 * @returns mouse position in vec2
	 */
	vec2 getMousePositionImpl() override;
	
	/**
	 * Get the X-Coordinate of the mouse position in the window. The value ranges from 0 to 1.
	 * @returns X-Coordinate of mouse position
	 */
	float getMouseXImpl() override;

    /**
     * Get the Y-Coordinate of the mouse position in the window. The value ranges from 0 to 1.
     * @returns Y-Coordinate of mouse position
     */
	float getMouseYImpl() override;
};

}

#endif