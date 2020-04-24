#include "events.h"
#include "platform.h"
#include "window.h"



namespace pine {

	void ResetHover() {
		Widget::hoverTime = 0;
		Widget::windowStack.back()->ResetHover();
	}

	void DispatchKeyEvent(int key) {
		KeyEvent e(key, Widget::modifiers);
		Widget::windowStack.back()->KeyPressEvent(e);
	}

	void DispatchCharEvent(unsigned code) {
		CharEvent e(code, Widget::modifiers);
		Widget::windowStack.back()->CharPressEvent(e);
	}

	void DispatchMousePressEvent() {
		MouseEvent e(Widget::mousePos);
		e.button = (CiMouseButton)Widget::mouseButtons;
		e.modifier = (CiMouseModifier)Widget::modifiers;
		Widget::windowStack.back()->MousePressEvent(e);
		ResetHover();
	}

	void DispatchMouseReleaseEvent(bool dblclk) {
		MouseEvent e(Widget::mousePos);
		e.dblclk = dblclk;
		e.button = (CiMouseButton)Widget::mouseButtons;
		e.modifier = (CiMouseModifier)Widget::modifiers;
		Widget::windowStack.back()->MouseReleaseEvent(e);
		ResetHover();
	}

	void DispatchMouseMoveEvent(double x, double y) {
		Widget::mousePos = Point(x, y);
		MouseEvent e(Widget::mousePos, (CiMouseButton)Widget::mouseButtons, (CiMouseModifier)Widget::modifiers);
		Widget::windowStack.back()->MouseMoveEvent(e);
		ResetHover();
	}

	void DispatchMouseHoverEvent() {
		if (++Widget::hoverTime == 20) {
			MouseEvent e(Widget::mousePos);
			Widget::windowStack.back()->MouseHoverEvent(e);
		}
	}

	void DispatchScrollEvent(double scroll) {
		MouseEvent e(Widget::mousePos, scroll);
		for (std::vector<Window *>::reverse_iterator i = Widget::windowStack.rbegin(); i != Widget::windowStack.rend(); ++i) {
			Widget::windowStack.back()->MouseScrollEvent(e);
			if (e.accepted) break;
		}
		ResetHover();
	}

	void DispatchUpdateEvent() {
		for (std::vector<Window *>::iterator i = Widget::windowStack.begin(); i != Widget::windowStack.end(); ++i) {
			UpdateEvent e(Window::update);
			(*i)->Update(e);
		}
	}

	void DispatchPaintEvent() {
		if (Widget::paint) {
			glViewport(0, 0, Widget::windowSize.x, Widget::windowSize.y);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, (GLdouble)Widget::windowSize.x, 0.0, (GLdouble)Widget::windowSize.y, -100.0, 100.0);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			for (std::vector<Window *>::iterator i = Widget::windowStack.begin(); i != Widget::windowStack.end(); ++i)
				(*i)->Paint();
			SwapBuffers();
			Widget::paint = false;
		}
	}

	void DispatchSizeEvent(double w, double h) {
		Widget::windowSize = Point(w, h);
		for (std::vector<Window *>::iterator i = Widget::windowStack.begin(); i != Widget::windowStack.end(); ++i)
			(*i)->Resize((*i)->parent ? (*i)->parent->Frame() : Rect(0, 0, w, h));
		Widget::paint = true;
		ResetHover();
	}

	void DispatchRefreshEvent() {
		Widget::paint = true;
		ResetHover();
	}

	void DispatchExitEvent() {
		if (Widget::windowStack.size() == 1)
			Widget::windowStack.back()->Exit(1);
	}
}
