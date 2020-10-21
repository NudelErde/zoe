/*
 * Input.h
 *
 *  Created on: 16.04.2019
 *      Author: florian
 */

#pragma once

#include "../zpch.h"
#include "../math/vec.h"

namespace Zoe {

/**
 * Input is used to get information about mouse and keyboard. All relevant methods are static.
 */
class DLL_PUBLIC Input {
public:

    /**
     * Destructs the Input object.
     */
    virtual ~Input() = default;

    /**
     * Check if a specific keycode is pressed. KeyCodes are defined in the KeyCode.h file. Keycode constants begin with KEY_
     * @param keycode is the checked keycode
     * @returns true if key is pressed
     */
    inline static bool isKeyPressed(int keycode) {
        return getInstance()->isKeyPressedImpl(keycode);
    }

    /**
     * Check if a specific mouse button is pressed. MouseCodes are defined in the MouseCode.h file. MouseCode constants begin with MOUSE_
     * @param button is the checked mouse button
     * @returns true if mouse is pressed
     */
    inline static bool isMouseButtonPressed(int button) {
        return getInstance()->isMouseButtonPressedImpl(button);
    }

    /**
     * Get the mouse position in the window. The values range from 0 to 1.
     * @returns mouse position in vec2
     */
    inline static vec2 getMousePosition() {
        return getInstance()->getMousePositionImpl();
    }

    /**
     * Get the X-Coordinate of the mouse position in the window. The value ranges from 0 to 1.
     * @returns X-Coordinate of mouse position
     */
    inline static float getMouseX() {
        return getInstance()->getMouseXImpl();
    }

    /**
     * Get the Y-Coordinate of the mouse position in the window. The value ranges from 0 to 1.
     * @returns Y-Coordinate of mouse position
     */
    inline static float getMouseY() {
        return getInstance()->getMouseYImpl();
    }

    /**
     * Returns an instance of the platform input.
     * @returns an instance of Input
     */
    static Input* getInstance();

protected:
    /**
     * Check if a specific keycode is pressed. KeyCodes are defined in the KeyCode.h file. Keycode constants begin with KEY_
     * @param keycode is the checked keycode
     * @returns true if key is pressed
     */
    virtual bool isKeyPressedImpl(int keycode) = 0;
    /**
     * Check if a specific mouse button is pressed. MouseCodes are defined in the MouseCode.h file. MouseCode constants begin with MOUSE_
     * @param button is the checked mouse button
     * @returns true if mouse is pressed
     */
    virtual bool isMouseButtonPressedImpl(int button) = 0;
    /**
     * Get the mouse position in the window. The values range from 0 to 1.
     * @returns mouse position in vec2
     */
    virtual vec2 getMousePositionImpl() = 0;
    /**
     * Get the X-Coordinate of the mouse position in the window. The value ranges from 0 to 1.
     * @returns X-Coordinate of mouse position
     */
    virtual float getMouseXImpl() = 0;
    /**
     * Get the Y-Coordinate of the mouse position in the window. The value ranges from 0 to 1.
     * @returns Y-Coordinate of mouse position
     */
    virtual float getMouseYImpl() = 0;
};

}
