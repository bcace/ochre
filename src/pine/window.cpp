#include "window.h"
#include "events.h"
#include "platform.h"
#include "pine/color.h"
#include <windows.h>

using namespace pine;



unsigned Window::update = 0;

Window::Window(Widget *_parent) : Widget(_parent),
	exitRequested(false),
	result(0) {}

int Window::Exec() {

	visible = true;
	Widget::windowStack.push_back(this);

	DispatchSizeEvent(Widget::windowSize.x, Widget::windowSize.y);	// initial resize of the window

	exitRequested = false;

	while (!exitRequested) {
		PollEvents();
		DispatchUpdateEvent();
		DispatchMouseHoverEvent();
		DispatchPaintEvent();
		Sleep(20);
		++Window::update;
	}

	Widget::windowStack.pop_back();
	visible = false;

	OnExit();

	Widget::paint = true;

	return result;
}

bool Window::Exit(int _result) {

	if (ExitTest()) {
		exitRequested = true;
		result = _result;
	}

	return exitRequested;
}

bool Window::ExitTest() {
	return true;
}

void Window::Paint() {

	RETURN_IF_INVISIBLE

	Schema::ApplyColor(scoBkgr);
	DrawRect(frame.min.x, frame.min.y, frame.max.x, frame.max.y);
	Widget::Paint();
}
