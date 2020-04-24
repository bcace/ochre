#include "widgets/welcome.h"
#include "pine/fonts/font.h"
#include "pine/color.h"
#include "pine/platform.h"
#include "session.h"

using namespace pine;



Seshwidget::Seshwidget(Widget *_parent) : Popup(_parent),
	list(this, false, List::IconAndText),
	closeButton(this, Button::TextOnly) {

	AddAction("Close", "", 0, &Seshwidget::LoadAction, KeyShortcut(keyEnter));
	AddAction("Exit", "", -1, &Seshwidget::ExitAction, KeyShortcut(keyEscape));

	closeButton.Connect(actions["Close"], Button::Clicked);
	list.Connect(actions["Close"], List::Choose);
}

int Seshwidget::SelectedIndex() {
	return list.SelectedIndex();
}

int Seshwidget::Show() {

	list.Clear();
	for (CiSessions::iterator s = CiSessions::ins.begin(); s != CiSessions::ins.end(); ++s)
		list.Add(ListItem(5, s->ToString(), Tag()));

	ResizedCentered(80.0, 40.0);
	return Popup::Exec();
}

void Seshwidget::Resize(const Rect &_frame) {

	Popup::Resize(_frame);

	list.Resize(Rect(
		frame.min.x + margin.x, frame.min.y + margin.y * 2.0 + DIR_BTN_H,
		frame.max.x - margin.x, frame.max.y - margin.y * 2.0 - DIR_BTN_H));

	closeButton.Resize(Rect(
		frame.max.x - margin.x - DIR_BTN_W,
		frame.min.y + margin.y,
		frame.max.x - margin.x,
		frame.min.y + margin.y + DIR_BTN_H
		));
}

void Seshwidget::Paint() {

	RETURN_IF_INVISIBLE

	Popup::Paint();

	EnableScissor();

	Schema::ApplyColor(scoDisabledFrgr);
	CiFont::Small().Bind().DrawText(
		"Your recent sessions",
		frame.Center().x,
		frame.max.y - (margin.y * 2.0 + DIR_BTN_H) * 0.5,
		taAll);

	DisableScissor();
}

//
// actions
//

void Seshwidget::LoadAction() {
	if (list.SelectedIndex() != -1)
		Exit(1);
}

void Seshwidget::ExitAction() {
	Exit(0);
}
