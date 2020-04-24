#ifndef scroll_h
#define scroll_h

#include "pine/control.h"

#define SCROLL_W	3.0



namespace pine {

	class CiScroll : public Control
	{
	public:
		CiScroll(Widget *);

		double Val() const;
		void SetVal(double);
		void IncreaseVal(double);
		double Content() const;
		void SetContent(double);

	protected:
		double val, content, size;

		void FixVal();
	};



	class CiVScroll : public CiScroll
	{
	public:
		CiVScroll(Widget *);
		void Resize(const Rect &);
		void Paint();
	};



	class CiHScroll : public CiScroll
	{
	public:
		CiHScroll(Widget *);
		void Resize(const Rect &);
		void Paint();
	};
}

#endif
