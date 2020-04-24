#include "platform.h"
#include "events.h"
#include "window.h"


GLFWwindow* window;

namespace pine {

	bool CreateWindow(const char *title, unsigned w, unsigned h) {

		Widget::windowSize = Point(w, h);

		if (!glfwInit())
			return false;

		window = glfwCreateWindow(Widget::windowSize.x, Widget::windowSize.y, title, NULL, NULL);

		if (!window) {
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, GLFWKeyCallback);
		glfwSetCharCallback(window, GLFWCharCallback);
		glfwSetWindowSizeCallback(window, GLFWSizeCallback);
		glfwSetWindowRefreshCallback(window, GLFWRefreshCallback);
		glfwSetMouseButtonCallback(window, GLFWMousebuttonCallback);
		glfwSetCursorPosCallback(window, GLFWMouseposCallback);
		glfwSetScrollCallback(window, GLFWScrollCallback);
		glfwSetWindowCloseCallback(window, GLFWCloseCallback);

		glfwSetTime(0.0);

		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPointSize(5.0);

		return true;
	}

	void DestroyWindow() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void PollEvents() {
		glfwPollEvents();
	}

	void SwapBuffers() {
		glfwSwapBuffers(window);
	}

	void SetClipboardString(const char *s) {
		glfwSetClipboardString(window, s);
	}

	const char *GetClipboardString() {
		return glfwGetClipboardString(window);
	}


	void GLFWKeyCallback(GLFWwindow *, int key, int, int action, int mods) {

		Widget::modifiers = 0;
		if (mods & GLFW_MOD_CONTROL)
			Widget::modifiers |= mmCtrl;
		if (mods & GLFW_MOD_SHIFT)
			Widget::modifiers |= mmShift;
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
			DispatchKeyEvent(key);
	}

	void GLFWCharCallback(GLFWwindow *, unsigned int code) {
		DispatchCharEvent(code);
	}

	void GLFWSizeCallback(GLFWwindow *, int w, int h) {
		DispatchSizeEvent(w, h);
	}

	void GLFWRefreshCallback(GLFWwindow *) {
		DispatchRefreshEvent();
	}

	void GLFWMousebuttonCallback(GLFWwindow *, int button, int action, int mods) {

		Widget::mouseButtons = (button == GLFW_MOUSE_BUTTON_1) ? mbLeft : mbRight;
		Widget::modifiers = mmNone;
		Widget::modifiers |= (mods & GLFW_MOD_CONTROL) ? mmCtrl : mmNone;
		Widget::modifiers |= (mods & GLFW_MOD_SHIFT) ? mmShift : mmNone;

		if (action == GLFW_PRESS)
			DispatchMousePressEvent();
		else if (action == GLFW_RELEASE) {
			bool dblclk = glfwGetTime() < 0.5;
			glfwSetTime(0.0);
			DispatchMouseReleaseEvent(dblclk);
			Widget::mouseButtons = mbNone;
		}
	}

	void GLFWMouseposCallback(GLFWwindow *, double x, double y) {
		DispatchMouseMoveEvent(x, Widget::windowSize.y - y);
	}

	void GLFWScrollCallback(GLFWwindow *, double x, double y) {
		DispatchScrollEvent(y);
	}

	void GLFWCloseCallback(GLFWwindow *) {
		DispatchExitEvent();
	}

}
