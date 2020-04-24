#ifndef files_h
#define files_h

#include "pine/popup.h"
#include "pine/widgets/button.h"
#include "pine/widgets/list.h"
#include "pine/widgets/edit.h"
#include "pine/path.h"



namespace pine {

	typedef std::vector<std::string> CiStrings;

	enum CiFilesMode { cfmOpen = 0, cfmSave };

	class Files : public Popup
	{
	public:
		enum CiFilesActions { Up = 0, Close, Ok, Enter };
		enum CiFilesType { Dir = 1, File = 2 };

		Files(Widget *, CiFilesMode);

		std::string Path() const;
		void SetPath(const std::string &);
		CiStrings SelectedFiles();
		std::string NewFile() const;
		void RefreshItems(const std::string &);

		// filesactions
		void OkAction();
		void UpAction();
		void CloseAction();

		void Resize(const Rect &);

	private:
		CiPath path;
		Button upBtn, closeBtn, okBtn;
		List list;
		Edit edit;
		CiFilesMode mode;
		double editH;
	};
}

#endif
