#ifndef tooltip_h
#define tooltip_h

#include "pine/control.h"



namespace pine {

	class Tooltip : public Control
	{
	public:
		Tooltip(Widget *);

		void Show(const std::string &, const Point &, bool);
		bool Hide();

		void Resize(const Rect &);
		void Paint();

	private:
		std::string text;
		bool multiline;
		double TP_MARGIN;
		Rect globalFrame;
	};
}

#endif
