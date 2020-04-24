#ifndef window_h
#define window_h

#include "widget.h"



namespace pine {

	class Window : public Widget
	{
	public:
		static unsigned update;

		void Paint();

		int Exec();
		bool Exit(int);

		virtual bool ExitTest();
		virtual void OnExit() {}
		virtual void ResetHover() {}

	protected:
		Window(Widget *);

	private:
		bool exitRequested;
		int result;
	};
}

#endif
