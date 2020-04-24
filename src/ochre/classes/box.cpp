#include "classes/box.h"
#include "runtime/frame.h"
#include "descriptors/class.h"
#include "ochre.h"
#include <cstdlib>
#include <math.h>



OcClass *CreateBoxClass() {
	return new OcClass("box", sizeof(OcBox));
}

void InitBoxClass(Ochre *ochre) {
	OcClass *boxCl = ochre->Class("box");
	OcClass *pointCl = ochre->Class("point");
	OcClass *realCl = ochre->Class("real");
	OcClass *boolCl = ochre->Class("bool");

	OcBox b;

	// instance vars

	boxCl->vars.Add(OcVar("min", pointCl, (char *)&b.min - (char *)&b));
	boxCl->vars.Add(OcVar("max", pointCl, (char *)&b.max - (char *)&b));

	// instance funcs

	boxCl->fns.push_back(OcFunc("init", "", OcArg(boxCl, ptCaller), ptModify, BoxSet, true));
	boxCl->fns.back().args.push_back(OcArg("dx", realCl, ptRead));
	boxCl->fns.back().args.push_back(OcArg("dy", realCl, ptRead));
	boxCl->fns.back().args.push_back(OcArg("dz", realCl, ptRead));

	boxCl->fns.push_back(OcFunc("setSize", "", OcArg(boxCl, ptCaller), ptModify, BoxSetSize, true));
	boxCl->fns.back().args.push_back(OcArg("dx", realCl, ptRead));
	boxCl->fns.back().args.push_back(OcArg("dy", realCl, ptRead));
	boxCl->fns.back().args.push_back(OcArg("dz", realCl, ptRead));

	boxCl->fns.push_back(OcFunc("randPoint", "", OcArg(pointCl, ptReadModify), ptRead, BoxRandPoint));
	boxCl->fns.push_back(OcFunc("midPoint", "", OcArg(pointCl, ptReadModify), ptRead, BoxMidPoint));

	boxCl->fns.push_back(OcFunc("clamp", "", OcArg(boolCl, ptRead), ptRead, BoxClamp));
	boxCl->fns.back().args.push_back(OcArg("position", pointCl, ptRead));

	boxCl->fns.push_back(OcFunc("simpleBounce", "", OcArg(0, ptNone), ptRead, BoxSimpleBounce));
	boxCl->fns.back().args.push_back(OcArg("position", pointCl, ptRead));
	boxCl->fns.back().args.push_back(OcArg("direction", pointCl, ptRead));

	boxCl->fns.push_back(OcFunc("teleport", "", OcArg(0, ptNone), ptRead, BoxTeleport));
	boxCl->fns.back().args.push_back(OcArg("position", pointCl, ptRead));
}

// instance funcs

OCF(BoxSet) {
	OCP(OcBox, 0) = OcBox(OcPoint(OCP(double, 1), OCP(double, 2), OCP(double, 3)));
}

OCF(BoxSetSize) {
	OCP(OcBox, 0).SetSize(OCP(double, 1), OCP(double, 2), OCP(double, 3));
}

OCF(BoxRandPoint) {
	OCP(OcPoint, ARG_RES) = OCP(OcBox, 0).RandPoint();
}

OCF(BoxMidPoint) {
	OCP(OcPoint, ARG_RES) = OCP(OcBox, 0).MidPoint();
}

OCF(BoxClamp) {
	OCP(bool, ARG_RES) = OCP(OcBox, 0).Clamp(OCP(OcPoint, 1));
}

OCF(BoxSimpleBounce) {
	OCP(OcBox, 0).SimpleBounce(OCP(OcPoint, 1), OCP(OcPoint, 2));
}

OCF(BoxTeleport) {
	OCP(OcBox, 0).Teleport(OCP(OcPoint, 1));
}



OcBox::OcBox() {}

OcBox::OcBox(double d) : max(d) {}

OcBox::OcBox(const OcPoint &_max) : max(_max) {}

OcBox::OcBox(const OcPoint &_min, const OcPoint &_max) : min(_min), max(_max) {}

void OcBox::SetSize(double dx, double dy, double dz) {
	max = min + OcPoint(dx, dy, dz);
}

OcPoint OcBox::RandPoint() {
	OcPoint r = max - min;

	return min + OcPoint(
		((double)rand() / (double)RAND_MAX) * r.x,
		((double)rand() / (double)RAND_MAX) * r.y,
		((double)rand() / (double)RAND_MAX) * r.z);
}

OcPoint OcBox::MidPoint() {
	return (min + max) * 0.5;
}

bool OcBox::Contains(const OcPoint &p) {
	return p >= min && p <= max;
}

bool OcBox::Clamp(OcPoint &pos) {

	bool modified = false;

	if (pos.x < min.x) {
		pos.x = min.x;
		modified = true;
	}
	else if (pos.x > max.x) {
		pos.x = max.x;
		modified = true;
	}

	if (pos.y < min.y) {
		pos.y = min.y;
		modified = true;
	}
	else if (pos.y > max.y) {
		pos.y = max.y;
		modified = true;
	}

	if (pos.z < min.z) {
		pos.z = min.z;
		modified = true;
	}
	else if (pos.z > max.z) {
		pos.z = max.z;
		modified = true;
	}

	return modified;
}

void OcBox::SimpleBounce(OcPoint &pos, OcPoint &dir) {

	if (pos.x < min.x) {
		pos.x = min.x;
		dir.x = -dir.x;
	}
	else if (pos.x > max.x) {
		pos.x = max.x;
		dir.x = -dir.x;
	}

	if (pos.y < min.y) {
		pos.y = min.y;
		dir.y = -dir.y;
	}
	else if (pos.y > max.y) {
		pos.y = max.y;
		dir.y = -dir.y;
	}

	if (pos.z < min.z) {
		pos.z = min.z;
		dir.z = -dir.z;
	}
	else if (pos.z > max.z) {
		pos.z = max.z;
		dir.z = -dir.z;
	}
}

void OcBox::Teleport(OcPoint &pos) {

	if (pos.x < min.x)
		pos.x = max.x;
	else if (pos.x > max.x)
		pos.x = min.x;

	if (pos.y < min.y)
		pos.y = max.y;
	else if (pos.y > max.y)
		pos.y = min.y;

	if (pos.z < min.z)
		pos.z = max.z;
	else if (pos.z > max.z)
		pos.z = min.z;
}

bool OcBox::operator!=(const OcBox &other) const {
	return min != other.min || max != other.max;
}

void OcBox::Absorb(const OcBox &b) {

	if (b.min.x < min.x)
		min.x = b.min.x;
	if (b.min.y < min.y)
		min.y = b.min.y;
	if (b.min.z < min.z)
		min.z = b.min.z;

	if (b.max.x > max.x)
		max.x = b.max.x;
	if (b.max.y > max.y)
		max.y = b.max.y;
	if (b.max.z > max.z)
		max.z = b.max.z;
}

void OcBox::UpdateCellIndices(double c, OcIndices &o, OcIndices &n) {

	if (c > 0.0) {

		if ((n.i = (int)(ceil(max.x / c) - (o.i = floor(min.x / c)))) <= 0)
			n.i = 1;

		if ((n.j = (int)(ceil(max.y / c) - (o.j = floor(min.y / c)))) <= 0)
			n.j = 1;

		if ((n.k = (int)(ceil(max.z / c) - (o.k = floor(min.z / c)))) <= 0)
			n.k = 1;
	}
	else {
		o.i = o.j = o.k = 0;
		n.i = n.j = n.k = 1;
	}
}

bool OcBox::IsValidBounds() const {
	return max.x >= min.x && max.y >= min.y && max.z >= min.z;
}

OcPoint OcBox::NearestDiagonalPoint(const OcPoint &p, bool clamp) const {

	OcPoint AP = p - min;
	OcPoint AB = max - min;

	double ab2 = AB.x * AB.x + AB.y * AB.y;
	double ap_ab = AP.x * AB.x + AP.y * AB.y;
	double t = ap_ab / ab2;

	if (clamp) {
		if (t < 0.0f) t = 0.0f;
		else if (t > 1.0f) t = 1.0f;
	}

	return min + (AB * t);
}
