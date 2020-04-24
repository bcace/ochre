#include "widgets/mainwidget.h"
#include "pine/widgets/messagebox.h"
#include "ochreapi.h"
#include "pine/platform.h"

using namespace pine;



Mainwidget::Mainwidget(const std::string &path) : Window(0),
	runtime(this),
	tab(this),
	newBtn(this, Button::IconAboveText, false),
	openBtn(this, Button::IconAboveText, false),
	saveBtn(this, Button::IconAboveText, false),
	seshBtn(this, Button::IconAboveText, false),
	runBtn(this, Button::IconAboveText, false),
	resetBtn(this, Button::IconAboveText, false),
	panesBtn(this, Button::IconAboveText, false),
	schemaBtn(this, Button::IconAboveText, false),
	issuesBtn(this, Button::IconAboveText, false),
	open(this, cfmOpen),
	save(this, cfmSave),
	tooltip(this),
	panes(mwpCombo),
	config(path) {

	OcCreate();

	Schema::CreateSchema();
	config.Deserialize();
	Schema::SelectScheme(config.GetString("scheme"));

	open.SetPath(config.GetString("openPath"));
	save.SetPath(config.GetString("newPath"));

	CiSessions::Deserialize();

	AddAction("New", "Create new ochre source file (Ctrl+N)", 8, &Mainwidget::NewAction, KeyShortcut(GLFW_KEY_N, true));
	AddAction("Open", "Load ochre source file (Ctrl+O)", 0, &Mainwidget::LoadAction, KeyShortcut(GLFW_KEY_O, true));
	AddAction("Save", "Save ochre source file (Ctrl+S)", 10, &Mainwidget::SaveAction, KeyShortcut(GLFW_KEY_S, true));
	AddAction("Sesh", "Show recent sessions (Ctrl+L)", 6, &Mainwidget::SeshAction, KeyShortcut(GLFW_KEY_L, true));
	AddAction("Run", "Run/stop simulation (F5)", 2, &Mainwidget::RunAction, KeyShortcut(keyF5));
	AddAction("Reset", "Reset simulation (Ctrl+R)", 4, &Mainwidget::ResetAction, KeyShortcut(GLFW_KEY_R, true));
	AddAction("Panes", "Toggle panes layout (Ctrl+T)", 5, &Mainwidget::ToggleAction, KeyShortcut(GLFW_KEY_T, true));
	AddAction("Schema", "Pick color scheme", 12, &Mainwidget::SchemaAction);
	AddAction("Issues", "List current parse issues", 7, &Mainwidget::IssuesAction, KeyShortcut(GLFW_KEY_I, true));
	AddAction("Quit", "Quit Cinnabar", -1, &Mainwidget::QuitAction, KeyShortcut(GLFW_KEY_Q, true));

	newBtn.Connect(actions["New"], Button::Clicked);
	openBtn.Connect(actions["Open"], Button::Clicked);
	saveBtn.Connect(actions["Save"], Button::Clicked);
	seshBtn.Connect(actions["Sesh"], Button::Clicked);
	runBtn.Connect(actions["Run"], Button::Clicked);
	resetBtn.Connect(actions["Reset"], Button::Clicked);
	panesBtn.Connect(actions["Panes"], Button::Clicked);
	schemaBtn.Connect(actions["Schema"], Button::Clicked);
	issuesBtn.Connect(actions["Issues"], Button::Clicked);
	issuesBtn.SetSpecColor(scoError);

	mainbar.
	AddExpander().
	AddWidget(newBtn).AddWidget(openBtn).AddWidget(saveBtn).AddWidget(seshBtn).
	AddSeparator().
	AddWidget(runBtn).AddWidget(resetBtn).
	AddSeparator().
	AddWidget(panesBtn).AddWidget(schemaBtn).AddWidget(issuesBtn).
	AddExpander();

	RefreshState();
}

bool Mainwidget::ExitTest() {

	Tags unsaved = tab.UnsavedFiles();
	if (unsaved.size() == 0)
		return true;

	std::string caption("Following files have unsaved changes:\n\n");
	for (Tags::iterator t = unsaved.begin(); t != unsaved.end(); ++t)
		caption += " - " + t->s + "\n";
	caption += "\ndo you want to save them?";

	Messagebox messagebox(this);
	switch (messagebox.Show(caption, Messagebox::AllActn)) {
		case Messagebox::Ok:
			tab.SaveAll();
			return true;
		case Messagebox::No:
			return true;
		case Messagebox::Cancel:
			paint = true;
			return false;
		default:
			return true;
	}
}

void Mainwidget::OnExit() {
	SaveSessions();
	SaveConfig();
	OcDelete();
}

void Mainwidget::SaveSessions() {
	CiSessions::Serialize();
}

void Mainwidget::SaveConfig() {
	config.Set("openPath", open.Path());
	config.Set("newPath", save.Path());
	config.Set("scheme", Schema::SelectedScheme().name);
	config.Serialize();
}

void Mainwidget::Resize(const Rect &_frame) {

	Widget::Resize(_frame);

	// menu buttons

	mainbar.Resize(Rect(frame.min.x, frame.min.y, frame.max.x, frame.min.y + MAIN_MENU_H));

	// layout panes

	if (panes == mwpCombo) {
		runtime.Resize(Rect(frame.min.x, frame.min.y + MAIN_MENU_H, frame.Center().x, frame.max.y));
		tab.Resize(Rect(frame.Center().x, frame.min.y + MAIN_MENU_H, frame.max.x, frame.max.y));
	}
	else if (panes == mwpSources)
		tab.Resize(Rect(frame.min.x, frame.min.y + MAIN_MENU_H, frame.max.x, frame.max.y));
	else
		runtime.Resize(Rect(frame.min.x, frame.min.y + MAIN_MENU_H, frame.max.x, frame.max.y));

	tooltip.Resize(frame);
}
