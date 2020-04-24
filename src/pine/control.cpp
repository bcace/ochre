#include "control.h"
#include "events.h"
#include "pine/action.h"

using namespace pine;



Control::Control(Widget *p) : Widget(p) {
	parent->RegisterControl(this);
}

Control::Control(Widget *p, int _icon, const std::string &_text) : Widget(p, _icon, _text) {
	parent->RegisterControl(this);
}

Control::Control(Widget *p, int _icon, const std::string &_text, const std::string &_desc) : Widget(p, _icon, _text, _desc) {
	parent->RegisterControl(this);
}

Control::~Control() {
	parent->UnregisterControl(this);
}

void Control::Connect(Action *action, int event) {

	if (action == 0)
		return;

	/*
	only allow subscribing widget's action to its own or its children events
	*/

	if (action->widget != this && !action->widget->IsParent(this))
		return;	// todo: log something here, for god's sake

	subscribedActions.insert(event, action);
}

void Control::Emit(const ActionEvent &event) {
	for (SubscribedActions::iterator i = subscribedActions.begin(event.action); i != subscribedActions.end(); ++i)
		i->Exec();
}
