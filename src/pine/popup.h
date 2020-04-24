#ifndef popup_h
#define popup_h

#include "window.h"



namespace pine {

	struct PopupResizerSingle
	{
		enum ResizerStyle {
			FixedMargin,
			FixedSize,
			AnchorLo,
			AnchorHi
		};

		double d1, d2;
		int style;

		void Resize(double, double, double &, double &);
	};



	class Popup : public Window
	{
	public:
		void ResizedCentered(double, double);
		void FixedCentered(double, double);
		void Pinned(double, double, double, double);
		void SlideDown(double, double);
		void SlideUp(double, double);

		void Resize(const Rect &);
		void Paint();

		void KeyPressEvent(KeyEvent &);
		void MouseReleaseEvent(MouseEvent &);

	protected:
		PopupResizerSingle xResizer, yResizer;
		Point margin;

		Popup(Widget *, const Point &_margin = Point(8.0, 8.0));
	};
}

#endif
