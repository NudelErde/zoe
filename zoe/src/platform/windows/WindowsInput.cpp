/*
 * WindowsInput.cpp
 *
 *  Created on: 16.04.2019
 *      Author: florian
 */

#ifdef WIN32

#include "WindowsInput.h"
#include "../../zoe/Application.h"
#include "GLFW/glfw3.h"

namespace Zoe{

static Input* instance = new WindowsInput();

Input* Input::getInstance() {
    return instance;
}

WindowsInput::~WindowsInput(){}

bool WindowsInput::isKeyPressedImpl(int keycode) {
	auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::isMouseButtonPressedImpl(int button) {
	auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

vec2 WindowsInput::getMousePositionImpl() {
	auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
	double x,y;
	glfwGetCursorPos(window, &x, &y);
	return {(float)x,(float)y};
}

float WindowsInput::getMouseXImpl() {
	vec2 tmp = this->getMousePositionImpl();
	return tmp.x;
}

float WindowsInput::getMouseYImpl() {
	vec2 tmp = this->getMousePositionImpl();
	return tmp.y;
}

}

#endif