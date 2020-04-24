#ifndef environment_h
#define environment_h

#include "descriptors/descriptors.h"
#include "classes/box.h"
#include "classes/file.h"



class Ochre;
class OcClass;

OcClass *CreateEnvClass();
void InitEnvClass(Ochre *);

OCF(EnvRestart);
OCF(EnvStepNo);
OCF(EnvRunNo);

OCF(EnvSetBoundsReal);
OCF(EnvSet3DGrid);

OCF(EnvRandPoint);
OCF(EnvMidPoint);
OCF(EnvContains);
OCF(EnvClamp);
OCF(EnvSimpleBounce);
OCF(EnvTeleport);

OCF(EnvSetPath);
OCF(EnvWriteln);
OCF(EnvWrite);

OCF(EnvCursorActive);
OCF(EnvNearestCursorRayPoint);



struct OcText;

struct OcEnvironment
{
	OcFile file;
	OcBox bounds, mouseRay;
	OcIndices o, n;
	double cellSize;
	bool restart;
//	, suspend;	!!!
	int step, run;

	OcEnvironment();

	void Clear();
	void Restart();

	void SetBounds(const OcBox &);
	void Set3DGrid(double);

	bool CursorActive();
	OcPoint NearestCursorRayPoint(const OcPoint &);
};

#endif
