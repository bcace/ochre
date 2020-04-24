#ifndef textarea_h
#define textarea_h

#include "pine/control.h"



namespace pine {

	class CiTextArea : public Control
	{
	public:
		CiTextArea(Widget *);

		const Point &TextSize() const;

		void SetText(const std::string &);

		void Paint();

	private:
		std::string text;
		Point margin;
		unsigned int rows, cols;
		Point textSize;
	};
}

#endif
