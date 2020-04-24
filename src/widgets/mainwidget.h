#ifndef mainwidget_h
#define mainwidget_h

#include "pine/window.h"
#include "widgets/runtimeview.h"
#include "pine/widgets/tab.h"
#include "pine/widgets/button.h"
#include "pine/widgets/files.h"
#include "pine/widgets/tooltip.h"
#include "pine/widgets/layout.h"
#include "pine/config.h"



namespace pine {

	class Mainwidget : public Window
	{
	public:
		enum MainWidgetPanes { mwpSources = 0x1, mwpRuntime = 0x2, mwpCombo = 0x3 };

		Mainwidget(const std::string &);

		bool ExitTest();
		void OnExit();

		void SaveSessions();
		void SaveConfig();
		void RefreshState();

		void NewAction();
		void LoadAction();
		void SaveAction();
		void SeshAction();
		void RunAction();
		void RunToAction();
		void ResetAction();
		void IssuesAction();
		void ToggleAction();
		void SchemaAction();
		void QuitAction();

		void Resize(const Rect &);
		void Paint();

		void MouseHoverEvent(MouseEvent &);
		void Update(UpdateEvent &);
		void ResetHover();

	private:
		RuntimeView runtime;
		Tab tab;
		Button newBtn, openBtn, saveBtn, seshBtn, runBtn, resetBtn, panesBtn, schemaBtn, issuesBtn;
		HLayout mainbar;
		Files open, save;
		Tooltip tooltip;
		MainWidgetPanes panes;
		Config config;
		Tags issues;
	};
}

#endif
