#ifndef container_h
#define container_h

#include "pine/control.h"

/*
this class exists only to draw the title bar.
*/


namespace pine {

	class CiContainer : public Control
	{
	public:
		CiContainer(Widget *p) : Control(p) {}

		void Paint();
	};
}

#endif
