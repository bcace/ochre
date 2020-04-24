#ifndef seshwidget_h
#define seshwidget_h

#include "pine/popup.h"
#include "pine/widgets/list.h"
#include "pine/widgets/button.h"



namespace pine {

	class Seshwidget : public Popup
	{
	public:
		Seshwidget(Widget *);

		int SelectedIndex();
		int Show();

		void Resize(const Rect &);
		void Paint();

		void LoadAction();
		void ExitAction();

	private:
		List list;
		Button closeButton;
	};
}

#endif
