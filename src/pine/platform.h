#ifndef platform_h
#define platform_h

#include "glad/glad.h"
#include "GLFW/glfw3.h"



namespace pine {

	/*
	functions that operate on the os and/or opengl boundary
	*/

	bool CreateWindow(const char *, unsigned, unsigned);
	void DestroyWindow();
	void PollEvents();
	void SwapBuffers();
	void SetClipboardString(const char *);
	const char *GetClipboardString();

	/*
	platform-specific key values
	*/

	enum PlatformKeys {
		keyEnter = GLFW_KEY_ENTER,
		keyEscape = GLFW_KEY_ESCAPE,
		keyF5 = GLFW_KEY_F5,
		keyUp = GLFW_KEY_UP,
		keyDown = GLFW_KEY_DOWN,
		keyLeft = GLFW_KEY_LEFT,
		keyRight = GLFW_KEY_RIGHT,
		keyTab = GLFW_KEY_TAB
	};

	/*
	platform-specific callbacks
	*/

	void GLFWKeyCallback(GLFWwindow *, int, int, int, int);
	void GLFWCharCallback(GLFWwindow *, unsigned int);
	void GLFWSizeCallback(GLFWwindow *, int, int);
	void GLFWRefreshCallback(GLFWwindow *);
	void GLFWMousebuttonCallback(GLFWwindow *, int, int, int);
	void GLFWMouseposCallback(GLFWwindow *, double, double);
	void GLFWScrollCallback(GLFWwindow *, double, double);
	void GLFWCloseCallback(GLFWwindow *);
}

#endif
