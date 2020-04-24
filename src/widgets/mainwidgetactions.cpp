#include "pine/widgets/popuplist.h"
#include "widgets/mainwidget.h"
#include "widgets/welcome.h"
#include "pine/color.h"
#include "pine/events.h"
#include "ochreapi.h"
#include <fstream>

using namespace pine;



void Mainwidget::NewAction() {

	if (save.Exec()) {

		std::string filename = save.NewFile();

		std::ofstream file(filename.c_str(), std::ofstream::trunc);
		if (!file.is_open())
			return;	// todo: notify user!

		file.close();

		if (tab.Add(filename))
			RefreshState();
	}
}

void Mainwidget::LoadAction() {

	if (open.Exec()) {

		CiStrings filenames = open.SelectedFiles();

		for (CiStrings::const_iterator f = filenames.begin(); f != filenames.end(); ++f)
			tab.Add(*f);

		RefreshState();
	}
}

void Mainwidget::SaveAction() {

	tab.SaveAll();

	RefreshState();
}

void Mainwidget::SeshAction() {

	Seshwidget welcome(this);
	if (welcome.Show()) {

		CiSessions::MakePopular(welcome.SelectedIndex());
		tab.LoadPopularSession();

		UpdateEvent e(Window::update);
		runtime.Update(e);

		RefreshState();
	}
}

void Mainwidget::RunAction() {

	switch (OcGetState()) {
		case OC_STATE_IDLE:
		case OC_STATE_SUSPENDED:
			OcRun();
		break;
		default:
			OcStop();
	}

	RefreshState();
	Widget::paint = true;
}

void Mainwidget::RunToAction() {

	switch (OcGetState()) {
		case OC_STATE_IDLE:
		case OC_STATE_SUSPENDED:
			{
				Tag t = tab.GetFocus();
				OcRunTo(t.i, t.j);
			}
		break;
		default:
			OcStop();
	}

	RefreshState();
}

void Mainwidget::ResetAction() {

	if (OcGetState()) {
		OcStop();
		OcReset();
		OcRun();
	}
	else
		OcReset();

	UpdateEvent e(Window::update);
	runtime.Update(e);

	RefreshState();
}

void Mainwidget::IssuesAction() {

	PopupList popup(this, false);
	for (Tags::iterator t = issues.begin(); t != issues.end(); ++t)
		popup.AddItem(t->s);

	int i = popup.Show(MAIN_MENU_H, 160.0, PopupList::Up);
	if (i != -1) {
		Tag &selIssue = issues[i];
		tab.FocusIssue(selIssue.i, selIssue.j);
	}
}

void Mainwidget::ToggleAction() {

	if (panes == mwpSources) {
		runtime.SetVisible(true);
		tab.SetVisible(true);
		panes = mwpCombo;
	}
	else if (panes == mwpCombo) {
		runtime.SetVisible(true);
		tab.SetVisible(false);
		panes = mwpRuntime;
	}
	else {
		runtime.SetVisible(false);
		tab.SetVisible(true);
		panes = mwpSources;
	}

	Resize(frame);
}

void Mainwidget::SchemaAction() {

	PopupList popup(this);
	for (Schema::iterator i = Schema::Begin(); i != Schema::End(); ++i)
		popup.AddItem(i->name);
	popup.SelectItem(Schema::SelectedScheme().id);

	int s = popup.Show(MAIN_MENU_H, 160.0, PopupList::Up);
	if (s != -1) {
		Schema::SelectScheme(s);
		Widget::paint = true;
	}
}

void Mainwidget::QuitAction() {
	Exit(0);
}
