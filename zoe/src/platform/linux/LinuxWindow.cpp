/*
 * WindowsWindow.cpp
 *
 *  Created on: 08.04.2019
 *      Author: florian
 */

#ifndef WIN32

#include "LinuxWindow.h"
#include "../../zoe/event/CommonEvent.h"
#include "../OpenGL/OpenGLContext.h"
#include "../../zoe/Console.h"

namespace Zoe {

static bool s_GLFWInitialized = false;
static bool s_GLEWInitialized = false;

static void GLFWErrorCallback(int errorCode, const char* description) {
	error("GLFW ERROR: ", description);
}

static void APIENTRY GLDebugMessageCallback(unsigned int source,
		unsigned int type, unsigned int id, unsigned int severity, int length,
		const char* message, const void* userParam) {
	error("GL_ERROR: ", message);
}

Window* Window::create(const WindowProps& props) {
	return new LinuxWindow(props);
}

void Window::shutdown() {
	//glfwTerminate();
}

LinuxWindow::LinuxWindow(const WindowProps& props) {
	init(props);
}

LinuxWindow::~LinuxWindow() {
	shutdown();
}

void LinuxWindow::init(const WindowProps& props) {
	data.title = props.title;
	data.width = props.width;
	data.height = props.height;

	Log::getLogger()->info("Creating Zoe Window");

	if (!s_GLFWInitialized) {
		int success = glfwInit();
		if (success != 1)
			critical("GLFWInit: ", success);

		glfwSetErrorCallback(GLFWErrorCallback);
		s_GLFWInitialized = true;
	}

	window = glfwCreateWindow((int) props.width, (int) props.height,
			props.title.c_str(), nullptr, nullptr);

	context = new OpenGLContext(window);
	context->init();

	glfwSetWindowUserPointer(window, &data);
	setVSync(true);

	//SET GLFW callbacks

	setUpListener();

	//GLEW Init

	if (!s_GLEWInitialized) {

		int error = glewInit();
		if (error) {
			critical("GLEWInit: ", error);
		}

		glDebugMessageCallback(&GLDebugMessageCallback, nullptr);

		s_GLEWInitialized = true;
	}

}

void LinuxWindow::setUpListener() {
	glfwSetWindowSizeCallback(window,
			[](GLFWwindow* window,int width,int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.width = width;
				data.height = height;

				WindowResizeEvent event(width,height);
				data.eventCallback(event);
			});
	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		WindowCloseEvent event;
		data.eventCallback(event);
	});

	glfwSetKeyCallback(window,
			[](GLFWwindow* window,int key,int scancode,int action ,int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key, 0);
						data.eventCallback(event);
					}
					break;
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.eventCallback(event);
					}
					break;
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, 1);
						data.eventCallback(event);
					}
					break;
				}
			});
	glfwSetMouseButtonCallback(window,
			[](GLFWwindow* window, int button, int action, int mods) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action) {
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.eventCallback(event);
					}
					break;
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.eventCallback(event);
					}
					break;
				}
			});
	glfwSetScrollCallback(window,
			[](GLFWwindow* window,double xOffset,double yOffset) {
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float) xOffset,(float)yOffset);
				data.eventCallback(event);
			});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window,double x,double y) {
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMovedEvent event((float) x,(float) y);
		data.eventCallback(event);
	});

}

void LinuxWindow::shutdown() {
	glfwDestroyWindow(window);
}

void LinuxWindow::onUpdate() {
	glfwPollEvents();
	context->swapBuffers();
}

void LinuxWindow::setVSync(bool enabled) {
	if (enabled) {
		glfwSwapInterval(1);
	} else {
		glfwSwapInterval(0);
	}
	data.vSync = enabled;
}

bool LinuxWindow::isVSync() const {
	return data.vSync;
}

}

#endif