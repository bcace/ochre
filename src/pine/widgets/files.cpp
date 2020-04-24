#include "pine/widgets/files.h"
#include "pine/path.h"
#include "pine/color.h"
#include "pine/fonts/font.h"
#include "pine/platform.h"

using namespace pine;



Files::Files(Widget *_parent, CiFilesMode _mode) : Popup(_parent),
	path("."),
	upBtn(this, Button::TextOnly),
	closeBtn(this, Button::TextOnly),
	okBtn(this, Button::TextOnly),
	list(this, _mode == cfmOpen, (List::ItemStyle)(List::IconAndText | List::Group | List::Alphabetize)),
	edit(this, "Enter file name here", false),
	mode(_mode),
	editH(0.0) {

	AddAction("Up", "", 3, &Files::UpAction);
	AddAction("OK", "", 1, &Files::OkAction, KeyShortcut(keyEnter));
	AddAction("Close", "", 0, &Files::CloseAction, KeyShortcut(keyEscape));

	upBtn.Connect(actions["Up"], Button::Clicked);
	okBtn.Connect(actions["OK"], Button::Clicked);
	closeBtn.Connect(actions["Close"], Button::Clicked);
	list.Connect(actions["OK"], List::Choose);

	edit.SetVisible(mode == cfmSave);
	if (mode == cfmSave)
		editH = CiFont::Normal().H() + TOP_MARGIN * 2.0;
}

std::string Files::Path() const {
	return path.Text();
}

void Files::SetPath(const std::string &_path) {
	path = CiPath(_path);
	RefreshItems("");
}

CiStrings Files::SelectedFiles() {

	CiStrings files;

	Tags tags = list.SelectedItems(Files::File);
	for (Tags::iterator t = tags.begin(); t != tags.end(); ++t)
		files.push_back(t->s);

	return files;
}

std::string Files::NewFile() const {
	return path.Text() + ((path.Text()[path.Text().size() - 1] == '/') ? "" : "/") + edit.Text();
}

void Files::RefreshItems(const std::string &sel) {

	list.Clear();

	CiPaths paths = path.Contents();
	for (CiPaths::iterator p = paths.begin(); p != paths.end(); ++p)
		list.Add(ListItem(
			p->IsDir() ? 3 : -1,
			p->Name(),
			Tag(p->IsDir(), p->Text()),
			p->Text() == sel,
			p->IsDir() ? Files::Dir : Files::File));
}

void Files::Resize(const Rect &_frame) {

	ResizedCentered(80.0, 40.0);
	Popup::Resize(_frame);

	// list

	list.Resize(Rect(
		frame.min.x + margin.x, frame.min.y + margin.y * 2.0 + DIR_BTN_H,
		frame.max.x - margin.x, frame.max.y - margin.y * 2.0 - DIR_BTN_H));

	// edit

	if (mode == cfmSave) {
		edit.Resize(Rect(
			frame.min.x + margin.x, frame.min.y + margin.y,
			frame.max.x - (margin.x + DIR_BTN_W) * 2.0 - 1.0, frame.min.y + margin.y + DIR_BTN_H));
	}

	// buttons

	Point pnt(frame.min.x + margin.x, frame.max.y - margin.y - DIR_BTN_H);
	upBtn.Resize(Rect(pnt, pnt + Point(DIR_BTN_W, DIR_BTN_H)));

	pnt = Point(frame.max.x - margin.x - DIR_BTN_W * 2, frame.min.y + margin.y);
	closeBtn.Resize(Rect(pnt, pnt + Point(DIR_BTN_W, DIR_BTN_H)));
	pnt.x += DIR_BTN_W + 1.0;
	okBtn.Resize(Rect(pnt, pnt + Point(DIR_BTN_W, DIR_BTN_H)));
}
