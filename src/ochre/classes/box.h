#ifndef box_h
#define box_h

#include "descriptors/descriptors.h"
#include "classes/point.h"



class Ochre;
class OcClass;

OcClass *CreateBoxClass();
void InitBoxClass(Ochre *);

OCF(BoxSet);
OCF(BoxSetSize);
OCF(BoxRandPoint);
OCF(BoxMidPoint);
OCF(BoxClamp);
OCF(BoxSimpleBounce);
OCF(BoxTeleport);


struct OcIndices
{
	int i, j, k;

	OcIndices() : i(0), j(0), k(0) {}

	OcIndices(int v) : i(v), j(v), k(v) {}

	bool operator!=(const OcIndices &o) {
		return i != o.i || j != o.j || k != o.k;
	}
};


struct OcBox
{
	OcPoint min, max;

	OcBox();
	OcBox(double);
	OcBox(const OcPoint &);
	OcBox(const OcPoint &, const OcPoint &);

	void SetSize(double, double, double);
	OcPoint RandPoint();
	OcPoint MidPoint();
	bool Contains(const OcPoint &);
	bool Clamp(OcPoint &);
	void SimpleBounce(OcPoint &, OcPoint &);
	void Teleport(OcPoint &);

	bool operator!=(const OcBox &) const;
	void Absorb(const OcBox &);
	void UpdateCellIndices(double, OcIndices &, OcIndices &);
	bool IsValidBounds() const;

	OcPoint NearestDiagonalPoint(const OcPoint &, bool) const;
};

#endif
