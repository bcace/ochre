#include "widgets/mainview.h"
#include "pine/platform.h"

using namespace pine;



CiAgent::CiAgent(int _id) : id(_id) {
	p[0] = p[1] = p[2] = 0.0;
}

CiAgent::CiAgent(int _id, double x, double y, double z) : id(_id) {
	p[0] = x; p[1] = y; p[2] = z;
}

CiAgent::CiAgent(int _id, double x, double y, double z, double _s) : id(_id), size(_s) {
	p[0] = x; p[1] = y; p[2] = z;
}

CiAgent::CiAgent(int _id, double x, double y, double z, double r, double g, double b) : id(_id) {
	p[0] = x; p[1] = y; p[2] = z;
	c[0] = r; c[1] = g; c[2] = b;
}

CiAgent::CiAgent(int _id, double x, double y, double z, double r, double g, double b, double _size) : id(_id), size(_size) {
	p[0] = x; p[1] = y; p[2] = z;
	c[0] = r; c[1] = g; c[2] = b;
	size = _size;
}

void CiAgent::Paint() {
	glVertex3dv(p);
}

void CiAgent::PaintForSelection() {
	glVertex3dv(p);
}

CiPool::CiPool(int _id) : id(_id), so(-1), co(-1) {}

CiSelection::CiSelection() : p(-1), a(-1) {}

CiSelection::CiSelection(int _p, int _a) : p(_p), a(_a) {}
