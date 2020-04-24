#ifndef tab_h
#define tab_h

#include "pine/widgets/container.h"
#include "pine/widgets/edit.h"
#include "pine/widgets/button.h"
#include "pine/widgets/layout.h"
#include "session.h"



namespace pine {

	class Tab : public CiContainer
	{
	public:
		enum TabActions { TypesActn = 0, CloseActn };

		Tab(Widget *);
		~Tab();

		bool Add(const std::string &);
		bool BuildEventPrologue();
		void Save();
		void SaveAll();
		Tags UnsavedFiles();
		void FocusIssue(int, int);
		Tag GetFocus();
		CiSession Session() const;
		void LoadPopularSession();
		void SetEditsEditable(bool);

		void Resize(const Rect &);
		void Paint();

	private:
		Button typesBtn, closeBtn;
		CiEdits edits;
		HLayout toolbar;
		int selEdit;
		bool forceBuild;

		void ShowTypesAction();
		void NextEditAction();
		void PreviousEditAction();
		void CloseCurrentEditAction();

		void ShowCurrentEdit();
		void CloseAllEdits();
		void FixSelection();
		Rect EditSize();
	};
}

#endif
