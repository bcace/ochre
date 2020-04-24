#ifndef messagebox_h
#define messagebox_h

#include "pine/popup.h"
#include "pine/widgets/button.h"
#include "pine/widgets/textarea.h"



namespace pine {

	class Messagebox : public Popup
	{
	public:
		enum CiMessageboxActions { Cancel = 0x0, Ok = 0x1, No = 0x2, AllActn = 0x3 };

		Messagebox(Widget *);

		int Show(const std::string &, CiMessageboxActions);

		void Resize(const Rect &);

		void CancelAction();
		void NoAction();
		void OkAction();

	private:
		Button cancelBtn, noBtn, okBtn;
		CiTextArea textArea;
	};
}

#endif
