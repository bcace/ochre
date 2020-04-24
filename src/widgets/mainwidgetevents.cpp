#include "widgets/mainwidget.h"
#include "ochreapi.h"
#include "pine/events.h"
#include "pine/platform.h"

using namespace pine;



void Mainwidget::MouseHoverEvent(MouseEvent &e) {

	if (frame.Contains(e.pos)) {

		Widget::MouseHoverEvent(e);

		if (e.accepted) {
			tooltip.Show(e.tag.s, e.pos, e.tag.b);
			return;
		}
	}
}

void Mainwidget::ResetHover() {
	tooltip.SetVisible(false);
}

void Mainwidget::Update(UpdateEvent &e) {

	switch (OcGetState()) {
	case OC_STATE_IDLE:
		break;
	case OC_STATE_RUNNING:
		OcPause();
		break;
	case OC_STATE_PAUSED:
		runtime.Update(e);
		OcResume();
		if (OcSelectFirstPool() == -1) {	// stop if there's no more pools in the env
			OcStop();
			OcReset();
			RefreshState();
			runtime.Update(e);
		}
		break;
	case OC_STATE_SUSPENDED:
		RefreshState();
		runtime.Update(e);
		break;
	default:;
	}

	// build event

	if (e.update % 50 == 0 && OcGetState() != OC_STATE_SUSPENDED) {

		if (tab.BuildEventPrologue()) {
			OcParse();

			// update issues here

			issues.clear();
			int id = OcSelectFirstType();
			while (id != -1) {

				int iCount = OcIssueCount();
				for (int i = 0; i < iCount; ++i) {

					const char *iText = OcGetIssueText(i);
					int iLoc = OcGetIssueLocation(i);

					issues.push_back(Tag(id, iText ? iText : "", iLoc));
				}

				id = OcSelectNextType();
			}

			RefreshState();
		}
	}
}
