#include "widgets/mainview.h"
#include "pine/events.h"
#include "pine/platform.h"
#include "pine/glub.h"
#include "ochreapi.h"

using namespace pine;



void Mainview::MousePressEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos))
		mousePos = e.pos;
}

void Mainview::MouseMoveEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE

	if (frame.Contains(e.pos)) {

		if (e.button == mbLeft) {

			if (e.modifier == mmNone) {

				rh -= (e.pos.y - mousePos.y) * 0.5;
				rv += (e.pos.x - mousePos.x) * 0.5;

				if (rh > 0.0)
					rh = 0.0;
				else if (rh < -180.0)
					rh = -180.0;

				e.accepted = true;
				Widget::paint = true;
			}
			else if (e.modifier == mmCtrl) {

				glub_unproject(e.pos.x, e.pos.y, 0.0,
					modelview, projection, viewport,
					&mouseRay[0], &mouseRay[1], &mouseRay[2]);

				glub_unproject(e.pos.x, e.pos.y, 1.0,
					modelview, projection, viewport,
					&mouseRay[3], &mouseRay[4], &mouseRay[5]);
			}
		}
		else if (e.button == mbRight) {

			ph -= e.pos.x - mousePos.x;
			pv -= e.pos.y - mousePos.y;

			e.accepted = true;
			Widget::paint = true;
		}

		mousePos = e.pos;
	}
}

void Mainview::MouseReleaseEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	mouseRay[0] = mouseRay[1] = mouseRay[2] = mouseRay[3] = mouseRay[4] = mouseRay[5] = 0.0;
}

void Mainview::MouseHoverEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (!frame.Contains(e.pos))
		return;

	int state = OcGetState();
	if (state != OC_STATE_IDLE && state != OC_STATE_SUSPENDED)
		return;

	CiSelection sel = PickTest(e.pos);
	if (sel.p != -1) {

		for (int p = OcSelectFirstPool(), pi = 0; p != -1; p = OcSelectNextPool(), ++pi) {

			if (pi == sel.p) {

				for (int a = OcSelectFirst(), ai = 0; a; a = OcSelectNext(), ++ai) {

					if (ai == sel.a) {

						e.tag.s = ComposeAgentTooltipText(sel);
						e.tag.b = true;
						e.accepted = true;
						Widget::paint = true;

						break;
					}
				}

				break;
			}
		}
	}
}

void Mainview::MouseScrollEvent(MouseEvent &e) {

	RETURN_IF_INVISIBLE_OR_ACCEPTED

	if (frame.Contains(e.pos)) {

		if (e.scroll > 0.0) {
			zoom /= 1.1;
			ph *= 1.1;
			pv *= 1.1;
		}
		else {
			zoom *= 1.1;
			ph /= 1.1;
			pv /= 1.1;
		}

		e.accepted = true;
		Widget::paint = true;
	}
}
