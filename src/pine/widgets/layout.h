#ifndef hlayout_h
#define hlayout_h

#include "pine/geometry.h"
#include <vector>



namespace pine {

	class Control;

	struct LayoutElement
	{
		bool separator, expander;
		Control *widget;
		double pos;	// separator only

		explicit LayoutElement() : separator(true), expander(false), widget(0) {}
		explicit LayoutElement(bool _expander) : separator(false), expander(_expander), widget(0) {}
		explicit LayoutElement(Control *w, bool _expander) : separator(false), expander(_expander), widget(w) {}
	};

	typedef std::vector<LayoutElement> LayoutElements;



	class HLayout
	{
	public:
		HLayout &AddWidget(Control &w, bool expand = false);
		HLayout &AddSeparator();
		HLayout &AddExpander();

		void Resize(const Rect &);
		void Paint();

	private:
		LayoutElements elems;
		Rect frame;
	};
}

#endif
