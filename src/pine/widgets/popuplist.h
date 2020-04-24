#ifndef popuplist_h
#define popuplist_h

#include "pine/popup.h"
#include "pine/widgets/list.h"



namespace pine {

	class PopupList : public Popup
	{
	public:
		enum Orientation { Down = 0, Up, Left, Right };

		PopupList(Widget *_parent, bool _permaselect = true);

		void AddItem(const std::string &);
		void SelectItem(int);

		int Show(double _offset, double _maxSize, Orientation _orientation = Down);

		void ChooseAction();

		void Resize(const Rect &);

	protected:
		List list;
		Orientation orientation;
		double offset, maxSize;
	};
}

#endif
