#include "widgets/mainwidget.h"
#include "ochreapi.h"

using namespace pine;



void Mainwidget::RefreshState() {

	bool loaded = OcSelectFirstType() != -1;
	bool idle = OcGetState() == OC_STATE_IDLE;
	bool hasIssues = issues.size() > 0;
	bool suspended = OcGetState() == OC_STATE_SUSPENDED;

	actions["New"]->enabled = idle;
	actions["Open"]->enabled = idle;
	actions["Save"]->enabled = loaded && !suspended;
	actions["Sesh"]->enabled = idle;
	actions["Run"]->enabled = loaded;
	actions["Run"]->text = (idle || suspended) ? "Run" : "Stop";
	actions["Run"]->icon = (idle || suspended) ? 2 : 3;
	actions["Issues"]->enabled = loaded && hasIssues;

	tab.SetEditsEditable(!suspended);
	Widget::paint = true;
}
