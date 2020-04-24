#include "widgets/mainwidget.h"
#include "widgets/mainview.h"
#include "pine/color.h"
#include "pine/platform.h"
#include "ochreapi.h"
#include <sstream>
#include <iostream>

using namespace pine;



Mainview::Mainview(Widget *p) : Control(p),
	rh(-60.0),
	rv(0.0),
	ph(0.0),
	pv(0.0),
	zoom(0.2) {

	mouseRay[0] = mouseRay[1] = mouseRay[2] = mouseRay[3] = mouseRay[4] = mouseRay[5] = 0.0;
}

std::string Mainview::ComposeAgentTooltipText(const CiSelection &sel) {

	std::stringstream ss;

	ss << pools[sel.p].name << " agent data:\n\n";

	for (int v = 0; ; ++v) {

		const char *name = OcGetVarName(v, 0);
		if (name == 0) break;

		const char *type = OcGetVarType(v, 0);
		if (type == 0) break;

		int off = OcGetVarOffset(name, type, 0);
		if (off == -1) break;

		void *data = OcGetData(off);
		if (data == 0) break;

		ss << ((v > 0) ? "\n" : "") << name << " (" << type << "): ";

		std::string t(type);
		if (t == "bool") {
			bool b = *(bool *)data;
			ss << b ? "true" : "false";
		}
		else if (t == "int") {
			int i = *(int *)data;
			ss << i;
		}
		else if (t == "real") {
			double d = *(double *)data;
			ss << d;
		}
		else if (t == "point") {
			Vertex p = *(Vertex *)data;
			ss << p.x << ", " << p.y << ", " << p.z;
		}
		else {
			ss << "--";
		}
	}

	return ss.str();
}

void Mainview::Paint() {

	Setup();

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	// draw agents

	glDepthMask(false);

	for (CiPools::iterator p = pools.begin(); p != pools.end(); ++p) {

		// draw agents

		if (p->sco == -1 && p->co == -1) {

			Schema::ApplyColor(scoFrgr);

			for (CiPool::iterator a = p->begin(); a != p->end(); ++a) {

				glPushMatrix();

				glTranslated(a->p[0], a->p[1], a->p[2]);
				glRotated(-rv, 0.0, 0.0, 1.0);
				glRotated(-rh, 1.0, 0.0, 0.0);

				glBegin(GL_QUADS);

				glVertex2d(-a->size, -a->size);
				glVertex2d(a->size, -a->size);
				glVertex2d(a->size, a->size);
				glVertex2d(-a->size, a->size);

				glEnd();
				glPopMatrix();
			}
		}
		else {

			for (CiPool::iterator a = p->begin(); a != p->end(); ++a) {

				glPushMatrix();

				glTranslated(a->p[0], a->p[1], a->p[2]);
				glRotated(-rv, 0.0, 0.0, 1.0);
				glRotated(-rh, 1.0, 0.0, 0.0);

				glColor3dv(a->c);

				glBegin(GL_QUADS);

				glVertex2d(-a->size, -a->size);
				glVertex2d(a->size, -a->size);
				glVertex2d(a->size, a->size);
				glVertex2d(-a->size, a->size);

				glEnd();
				glPopMatrix();
			}
		}
	}

	// bounds

	glLineWidth(2.0f);

	Schema::ApplyColor(scoFrgr, 20);

	glBegin(GL_LINE_LOOP);
	glVertex3d(bounds.x.min, bounds.y.min, bounds.z.min);
	glVertex3d(bounds.x.max, bounds.y.min, bounds.z.min);
	glVertex3d(bounds.x.max, bounds.y.max, bounds.z.min);
	glVertex3d(bounds.x.min, bounds.y.max, bounds.z.min);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3d(bounds.x.min, bounds.y.min, bounds.z.max);
	glVertex3d(bounds.x.max, bounds.y.min, bounds.z.max);
	glVertex3d(bounds.x.max, bounds.y.max, bounds.z.max);
	glVertex3d(bounds.x.min, bounds.y.max, bounds.z.max);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(bounds.x.min, bounds.y.min, bounds.z.min);
	glVertex3d(bounds.x.min, bounds.y.min, bounds.z.max);
	glVertex3d(bounds.x.max, bounds.y.min, bounds.z.min);
	glVertex3d(bounds.x.max, bounds.y.min, bounds.z.max);
	glVertex3d(bounds.x.max, bounds.y.max, bounds.z.min);
	glVertex3d(bounds.x.max, bounds.y.max, bounds.z.max);
	glVertex3d(bounds.x.min, bounds.y.max, bounds.z.min);
	glVertex3d(bounds.x.min, bounds.y.max, bounds.z.max);
	glEnd();

	// grid

	glLineWidth(1.0f);
	glBegin(GL_LINES);

	int oi = cells[0];
	int oj = cells[1];
	int ok = cells[2];
	int ni = oi + cells[3];
	int nj = oj + cells[4];
	int nk = ok + cells[5];

	for (int i = oi; i <= ni; ++i) {

		double x = i * cellSize;
		if (x > bounds.x.min && x < bounds.x.max) {

			glVertex3d(x, bounds.y.min, bounds.z.min);
			glVertex3d(x, bounds.y.max, bounds.z.min);

			glVertex3d(x, bounds.y.min, bounds.z.max);
			glVertex3d(x, bounds.y.max, bounds.z.max);

			glVertex3d(x, bounds.y.min, bounds.z.min);
			glVertex3d(x, bounds.y.min, bounds.z.max);

			glVertex3d(x, bounds.y.max, bounds.z.min);
			glVertex3d(x, bounds.y.max, bounds.z.max);
		}
	}

	for (int j = oj; j <= nj; ++j) {

		double y = j * cellSize;
		if (y > bounds.y.min && y < bounds.y.max) {

			glVertex3d(bounds.x.min, y, bounds.z.min);
			glVertex3d(bounds.x.max, y, bounds.z.min);

			glVertex3d(bounds.x.min, y, bounds.z.max);
			glVertex3d(bounds.x.max, y, bounds.z.max);

			glVertex3d(bounds.x.min, y, bounds.z.min);
			glVertex3d(bounds.x.min, y, bounds.z.max);

			glVertex3d(bounds.x.max, y, bounds.z.min);
			glVertex3d(bounds.x.max, y, bounds.z.max);
		}
	}

	for (int k = ok; k <= nk; ++k) {

		double z = k * cellSize;
		if (z > bounds.z.min && z < bounds.z.max) {

			glVertex3d(bounds.x.min, bounds.y.min, z);
			glVertex3d(bounds.x.min, bounds.y.max, z);

			glVertex3d(bounds.x.max, bounds.y.min, z);
			glVertex3d(bounds.x.max, bounds.y.max, z);

			glVertex3d(bounds.x.min, bounds.y.min, z);
			glVertex3d(bounds.x.max, bounds.y.min, z);

			glVertex3d(bounds.x.min, bounds.y.max, z);
			glVertex3d(bounds.x.max, bounds.y.max, z);
		}
	}

	glEnd();

	glDepthMask(true);

	Dismantle();

	Widget::Paint();
}

CiSelection Mainview::PickTest(const Point &pos) {

	CiSelection sel;

	if (!frame.Contains(pos))
		return sel;

	Setup();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw agents

	int pi = 0;
	for (CiPools::iterator p = pools.begin(); p != pools.end(); ++p, ++pi) {

		int ai = 0;
		for (CiPool::iterator a = p->begin(); a != p->end(); ++a, ++ai) {

			glPushMatrix();

			glTranslated(a->p[0], a->p[1], a->p[2]);
			glRotated(-rv, 0.0, 0.0, 1.0);
			glRotated(-rh, 1.0, 0.0, 0.0);

			glColor4ub(ai >> 8, ai & 0xFF, pi, 255);

			glBegin(GL_QUADS);

			glVertex2d(-a->size, -a->size);
			glVertex2d(a->size, -a->size);
			glVertex2d(a->size, a->size);
			glVertex2d(-a->size, a->size);

			glEnd();
			glPopMatrix();
		}
	}

	unsigned char pixel[4];
	glReadPixels(pos.x, pos.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	if (pixel[3]) {
		sel.p = pixel[2];
		sel.a = pixel[1] | (pixel[0] << 8);
	}

	Dismantle();

	Paint();

	return sel;
}

//
// private parts :)
//

void Mainview::Setup() {

	EnableScissor();
	ApplyViewport();

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glFrustum(
				0.00001 * zoom * (ph - frame.W() * 0.5),
				0.00001 * zoom * (ph + frame.W() * 0.5),
				0.00001 * zoom * (pv - frame.H() * 0.5),
				0.00001 * zoom * (pv + frame.H() * 0.5),
				0.001, 200.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// model transformation

	glTranslatef(0.0f, 0.0f, -100.0f);
	glRotated(rh, 1.0, 0.0, 0.0);
	glRotated(rv, 0.0, 0.0, 1.0);
	double scale = 100.0 / bounds.Diagonal();
	glScaled(scale, scale, scale);
	glTranslated(-bounds.x.Size() * 0.5, -bounds.y.Size() * 0.5, -bounds.z.Size() * 0.5);
}

void Mainview::Dismantle() {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glDisable(GL_DEPTH_TEST);

	DisableScissor();
	Widget::windowStack.front()->ApplyViewport();
}
