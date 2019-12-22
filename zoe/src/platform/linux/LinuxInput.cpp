/*
 * LinuxInput.cpp
 *
 *  Created on: 16.04.2019
 *      Author: florian
 */

#include "LinuxInput.h"
#include "../../zoe/Application.h"
#include "GLFW/glfw3.h"

namespace Zoe{

Input* Input::instance = new LinuxInput();

LinuxInput::~LinuxInput(){}

bool LinuxInput::isKeyPressedImpl(int keycode) {
	auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool LinuxInput::isMouseButtonPressedImpl(int button) {
	auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
	auto state = glfwGetMouseButton(window, button);
	return state == GLFW_PRESS;
}

vec2 LinuxInput::getMousePositionImpl() {
	auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
	double x,y;
	glfwGetCursorPos(window, &x, &y);
	return {(float)x,(float)y};
}

float LinuxInput::getMouseXImpl() {
	vec2 tmp = this->getMousePositionImpl();
	return tmp.x;
}

float LinuxInput::getMouseYImpl() {
	vec2 tmp = this->getMousePositionImpl();
	return tmp.y;
}

}
