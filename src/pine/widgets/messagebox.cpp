#include "pine/widgets/messagebox.h"
#include "pine/color.h"
#include "pine/fonts/font.h"
#include "pine/platform.h"

using namespace pine;



Messagebox::Messagebox(Widget *_parent) : Popup(_parent),
	cancelBtn(this, Button::TextOnly),
	noBtn(this, Button::TextOnly),
	okBtn(this, Button::TextOnly),
	textArea(this) {

	AddAction("Cancel", "", 0, &Messagebox::CancelAction, KeyShortcut(keyEscape));
	AddAction("No", "", 0, &Messagebox::NoAction);
	AddAction("OK", "", 0, &Messagebox::OkAction, KeyShortcut(keyEnter));

	cancelBtn.Connect(actions["Cancel"], Button::Clicked);
	noBtn.Connect(actions["No"], Button::Clicked);
	okBtn.Connect(actions["OK"], Button::Clicked);
}

int Messagebox::Show(const std::string &text, CiMessageboxActions) {
	textArea.SetText(text);
	Point size = textArea.TextSize() + Point(margin.x * 2.0, margin.y * 3.0 + DIR_BTN_H);
	FixedCentered(size.x, size.y);
	return Exec();
}

void Messagebox::Resize(const Rect &_frame) {

	Popup::Resize(_frame);

	textArea.Resize(Rect(
		frame.min.x + margin.x,
		frame.min.y + 2.0 * margin.y + DIR_BTN_H,
		frame.max.x - margin.x,
		frame.max.y - margin.y
		));

	double x = frame.max.x - margin.x, y = frame.min.y + margin.y;
	okBtn.Resize(Rect(x - DIR_BTN_W, y, x, y + DIR_BTN_H));
	x -= DIR_BTN_W + 1;
	noBtn.Resize(Rect(x - DIR_BTN_W, y, x, y + DIR_BTN_H));
	x -= DIR_BTN_W + 1;
	cancelBtn.Resize(Rect(x - DIR_BTN_W, y, x, y + DIR_BTN_H));
}

void Messagebox::CancelAction() {
	Exit(Cancel);
}

void Messagebox::NoAction() {
	Exit(No);
}

void Messagebox::OkAction() {
	Exit(Ok);
}
