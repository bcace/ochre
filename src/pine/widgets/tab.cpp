#include "pine/widgets/messagebox.h"
#include "pine/widgets/popuplist.h"
#include "pine/widgets/tab.h"
#include "pine/fonts/font.h"
#include "pine/color.h"
#include "pine/platform.h"
#include "ochreapi.h"

#define PINE_EMPTY_TAB_CAPTION	"..."

using namespace pine;



Tab::Tab(Widget *p) : CiContainer(p),
	typesBtn(this, Button::IconAndText, false),
	closeBtn(this, Button::IconOnly, false),
	selEdit(-1),
	forceBuild(false) {

	AddAction("Close", "Close the editor & unload type", 4, &Tab::CloseCurrentEditAction, KeyShortcut('w', true));
	AddAction(PINE_EMPTY_TAB_CAPTION, "Show loaded types", 3, &Tab::ShowTypesAction);
	AddAction("Next", "Show next type", -1, &Tab::NextEditAction, KeyShortcut(keyTab, true));
	AddAction("Previous", "Show previous type", -1, &Tab::PreviousEditAction, KeyShortcut(keyTab, true, true));

	typesBtn.Connect(actions[PINE_EMPTY_TAB_CAPTION], Button::Clicked);
	closeBtn.Connect(actions["Close"], Button::Clicked);
	closeBtn.SetSize(TOOLBAR_H, TOOLBAR_H);

	toolbar.
	AddSeparator().
	AddWidget(typesBtn, true).
	AddSeparator().
	AddWidget(closeBtn);
}

Tab::~Tab() {
	for (CiEdits::iterator e = edits.begin(); e != edits.end(); ++e)
		delete *e;
}

void Tab::ShowTypesAction() {

	if (selEdit == -1) return;

	PopupList list(this);
	for (unsigned int i = 0; i < edits.size(); ++i)
		list.AddItem(edits[i]->GetTag().s);
	list.SelectItem(selEdit);

	int t = list.Show(frame.max.y - typesBtn.Frame().min.y, 80.0, PopupList::Down);
	if (t != -1) {
		selEdit = t;
		ShowCurrentEdit();
	}
}

void Tab::NextEditAction() {
	++selEdit;
	FixSelection();
	ShowCurrentEdit();
}

void Tab::PreviousEditAction() {
	--selEdit;
	FixSelection();
	ShowCurrentEdit();
}

void Tab::CloseCurrentEditAction() {

	if (selEdit == -1) return;

	if (edits[selEdit]->Unsaved()) {

		Messagebox messagebox(this);
		switch (messagebox.Show("This file has some unsaved changes.\n\nDo you want to save them before closing?", Messagebox::AllActn)) {
			case Messagebox::Cancel:	return;
			case Messagebox::Ok:		Save(); break;
			case Messagebox::No:		break;
			default:;
		}
	}

	CiEdits::iterator e = edits.begin() + selEdit;

	delete *e;
	edits.erase(e);

	FixSelection();
	ShowCurrentEdit();

	forceBuild = true;
}

bool Tab::Add(const std::string &path) {

	int id = OcAddType(path.c_str(), OC_ADDTYPE_SOURCE_PATH);
	if (id == OC_NO_INS)
		return false;

	Tag t(id, path);

	Edit *e = new Edit(this, t);
	e->Resize(EditSize());
	e->LoadFromFile(t.s);
	edits.push_back(e);
	selEdit = edits.size() - 1;

	ShowCurrentEdit();

	return true;
}

bool Tab::BuildEventPrologue() {

	bool needsParse = forceBuild;
	forceBuild = false;

	for (CiEdits::iterator e = edits.begin(); e != edits.end(); ++e)
		if ((*e)->BuildEventPrologue())
			needsParse = true;

	return needsParse;
}

void Tab::FocusIssue(int id, int loc) {

	for (unsigned int i = 0; i < edits.size(); ++i) {
		if (edits[i]->GetTag().i == id) {
			selEdit = i;
			ShowCurrentEdit();
			edits[selEdit]->FocusLocation(loc);
			break;
		}
	}
}

Tag Tab::GetFocus() {
	return (selEdit != -1) ?
		Tag(edits[selEdit]->GetTag().i, edits[selEdit]->CaretPos()) :
		Tag(-1, -1);
}

CiSession Tab::Session() const {

	CiSession session;

	for (CiEdits::const_iterator e = edits.begin(); e != edits.end(); ++e)
		session.push_back((*e)->GetTag().s);

	return session;
}

void Tab::LoadPopularSession() {

	if (CiSessions::Session() == 0)
		return;

	OcReset();

	CloseAllEdits();

	for (CiSession::iterator s = CiSessions::Session()->begin(); s != CiSessions::Session()->end(); ++s)
		Add(*s);
}

void Tab::SetEditsEditable(bool editable) {
	for (CiEdits::const_iterator e = edits.begin(); e != edits.end(); ++e)
		(*e)->SetEditable(editable);
}

void Tab::ShowCurrentEdit() {

	for (int e = 0; e < (int)edits.size(); ++e)
		edits[e]->SetVisible(e == selEdit);

	typesBtn.Text() = (selEdit == -1) ? PINE_EMPTY_TAB_CAPTION : edits[selEdit]->GetTag().s;
}

void Tab::CloseAllEdits() {

	while (edits.size()) {
		delete edits.back();
		edits.pop_back();
		forceBuild = true;
	}
}

void Tab::Save() {

	if (selEdit == -1) return;

	edits[selEdit]->SaveToFile(edits[selEdit]->GetTag().s);

	CiSessions::MakePopular(Session());
}

void Tab::SaveAll() {
	for (CiEdits::iterator e = edits.begin(); e != edits.end(); ++e)
		(*e)->SaveToFile((*e)->GetTag().s);
	CiSessions::MakePopular(Session());
}

Tags Tab::UnsavedFiles() {

	Tags tags;

	for (CiEdits::iterator e = edits.begin(); e != edits.end(); ++e)
		if ((*e)->Unsaved())
			tags.push_back((*e)->GetTag());

	return tags;
}

void Tab::FixSelection() {

	if (edits.size() == 0)
		selEdit = -1;
	else if (selEdit < 0)
		selEdit = edits.size() - 1;
	else if (selEdit >= edits.size())
		selEdit = 0;
}

Rect Tab::EditSize() {
	return Rect(frame.min.x, frame.min.y, frame.max.x, frame.max.y - TOOLBAR_H - 4.0);
}

//
// widget stuff
//

void Tab::Resize(const Rect &_frame) {

	Widget::Resize(_frame);

	toolbar.Resize(Rect(frame.min.x, frame.max.y - TOOLBAR_H, frame.max.x, frame.max.y));

	for (CiEdits::iterator e = edits.begin(); e != edits.end(); ++e)
		(*e)->Resize(EditSize());
}

void Tab::Paint() {

	RETURN_IF_INVISIBLE

	CiContainer::Paint();

	// draw mainbar separators

	toolbar.Paint();
}
