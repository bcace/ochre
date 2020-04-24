#ifndef events_h
#define events_h

#include "geometry.h"
#include "widget.h"



namespace pine {

	class Event
	{
	public:
		bool accepted;
		Tag tag;

		Event() : accepted(false) {}
	};

	// system events (top to bottom)

	enum CiMouseButton { mbNone, mbLeft, mbRight };
	enum CiMouseModifier { mmNone = 0x0, mmCtrl = 0x1, mmShift = 0x2 };

	class MouseEvent : public Event
	{
	public:
		Point pos;
		CiMouseButton button;
		CiMouseModifier modifier;
		bool dblclk;
		double scroll;

		MouseEvent(const Point &_pos, CiMouseButton _button = mbNone, CiMouseModifier _modifier = mmNone) :
			pos(_pos), button(_button), modifier(_modifier), dblclk(false), scroll(0.0) {}

		MouseEvent(const Point &_pos, double _scroll) :
			pos(_pos), button(mbNone), modifier(mmNone), dblclk(false), scroll(_scroll) {}
	};



	class UpdateEvent : public Event
	{
	public:
		unsigned update;

		UpdateEvent(unsigned _update) :
			update(_update) {}
	};



	class KeyEvent : public Event
	{
	public:
		int key, mods;

		KeyEvent(int _key, int _mods) :
			key(_key), mods(_mods) {}
	};



	class CharEvent : public Event
	{
	public:
		unsigned int code;
		int mods;

		CharEvent(unsigned int _code, int _mods) :
			code(_code), mods(_mods) {}
	};



	class ActionEvent : public Event
	{
	public:
		int action;

		ActionEvent(int _action) :
			action(_action) {}
	};

	/*
	event dispatch
	*/

	void DispatchKeyEvent(int);
	void DispatchCharEvent(unsigned);
	void DispatchMousePressEvent();
	void DispatchMouseReleaseEvent(bool);
	void DispatchMouseMoveEvent(double, double);
	void DispatchMouseHoverEvent();
	void DispatchScrollEvent(double);
	void DispatchUpdateEvent();
	void DispatchPaintEvent();
	void DispatchSizeEvent(double, double);
	void DispatchRefreshEvent();
	void DispatchExitEvent();
}

#endif
