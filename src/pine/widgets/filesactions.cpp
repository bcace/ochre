#include "pine/widgets/files.h"

using namespace pine;



void Files::OkAction() {

	if (list.SelectedItems(Files::Dir).size()) {
		path = CiPath(list.SelectedItems(Files::Dir)[0].s);
		RefreshItems("");
	}
	else if (mode == cfmOpen) {
		if (list.SelectedItems(Files::File).size())
			Exit(1);
	}
	else if (mode == cfmSave) {
		if (edit.Text().size() > 0)
			Exit(1);
	}
}

void Files::UpAction() {

	std::string pathText = path.Text();
	path = CiPath(path.Location());
	RefreshItems(pathText);
}

void Files::CloseAction() {
	Exit(0);
}
