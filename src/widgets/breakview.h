#ifndef breakview_h
#define breakview_h

#include "pine/control.h"

/*
Breakview widget displays current frame states when
runtime environment is suspended.
*/

namespace pine {

	class CiBreakview : public Control
	{
	public:
		CiBreakview(Widget *p) : Control(p) {}

	private:
	};
}

#endif
