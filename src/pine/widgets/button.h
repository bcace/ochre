#ifndef button_h
#define button_h

#include "pine/control.h"
#include <map>

#define DIR_BTN_H	24.0
#define DIR_BTN_W	56.0



namespace pine {

	enum CiButtonState { bsNormal = 0, bsPressed, bsHover };

	class Button : public Control
	{
	public:
		enum ButtonEvent { Clicked = PI_FACE_EVENT };
		enum ButtonStyle { IconOnly, TextOnly, IconAndText, IconAboveText, LargeIconOnly };

		Button(Widget *_parent, ButtonStyle _style = IconOnly, bool _drawBkgr = true);
		Button(Widget *_parent, int _icon, const std::string &_text, ButtonStyle _style = IconOnly, bool _drawBkgr = true);
		Button(Widget *_parent, int _icon, const std::string &_text, const std::string &_desc, ButtonStyle _style = IconOnly, bool _drawBkgr = true);

		void SetSpecColor(int);

		void Paint();

		void MousePressEvent(MouseEvent &);
		void MouseMoveEvent(MouseEvent &);
		void MouseReleaseEvent(MouseEvent &);
		void MouseHoverEvent(MouseEvent &);

	protected:
		int specColor;
		bool drawBkgr, useSpecColor;
		CiButtonState state;
		ButtonStyle style;
	};
}

#endif
