/*
 * LinuxInput.cpp
 *
 *  Created on: 16.04.2019
 *      Author: florian
 */

#if !defined(WIN32) || defined(DOXYGEN)

#include "LinuxInput.h"
#include "../../zoe/core/Application.h"
#include "GLFW/glfw3.h"

namespace Zoe{

static Input* instance = new LinuxInput();

Input* Input::getInstance() {
    return instance;
}

LinuxInput::~LinuxInput() = default;

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
	return {(float)x/(float)Application::get().getWindow().getWidth(),(float)y/(float)Application::get().getWindow().getHeight()};
}

float LinuxInput::getMouseXImpl() {
	vec2 tmp = this->getMousePositionImpl();
	return (float)tmp.x;
}

float LinuxInput::getMouseYImpl() {
	vec2 tmp = this->getMousePositionImpl();
	return (float)tmp.y;
}

}

#endif