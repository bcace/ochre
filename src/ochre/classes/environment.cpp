#include "classes/environment.h"
#include "descriptors/class.h"
#include "ochre.h"



OcClass *CreateEnvClass() {
	return new OcClass("env", sizeof(OcEnvironment));
}

void InitEnvClass(Ochre *ochre) {
	OcClass *envCl = ochre->Class("env");
	OcClass *realCl = ochre->Class("real");
	OcClass *pointCl = ochre->Class("point");
	OcClass *boolCl = ochre->Class("bool");
	OcClass *textCl = ochre->Class("text");
	OcClass *intCl = ochre->Class("int");

	// static funcs

	envCl->sfns.push_back(OcFunc("restart", "restarts the simulation after the current step is done, 'set' section is executed", OcArg(0, ptNone), ptNone, EnvRestart, false, ktSet | ktInter | ktAct));

	envCl->sfns.push_back(OcFunc("stepNo", "returns the current simulation step, 0 - based, 'set' section counts as -1", OcArg(intCl, ptRead), ptNone, EnvStepNo));
	envCl->sfns.push_back(OcFunc("runNo", "returns the current run, 0 - based", OcArg(intCl, ptRead), ptNone, EnvRunNo));

	envCl->sfns.push_back(OcFunc("setBounds",
		"sets the bounding box dimensions with origin at { 0, 0, 0 }, bounding box is automatically refreshed before the new step, rebuilds the grid",
		OcArg(0, ptNone), ptNone, EnvSetBoundsReal, false, ktInter | ktAct));
	envCl->sfns.back().args.push_back(OcArg("max x", realCl, ptRead));
	envCl->sfns.back().args.push_back(OcArg("max y", realCl, ptRead));
	envCl->sfns.back().args.push_back(OcArg("max z", realCl, ptRead));

	envCl->sfns.push_back(OcFunc("set3DGrid",
		"sets the 3D grid cell size, grid is automatically rebuilt to fit the bounds before the next step",
		OcArg(0, ptNone), ptNone, EnvSet3DGrid, false, ktInter | ktAct));
	envCl->sfns.back().args.push_back(OcArg("cell size", realCl, ptRead));


	envCl->sfns.push_back(OcFunc("randPoint", "returns a random point within set bounds", OcArg(pointCl, ptReadModify), ptNone, EnvRandPoint));
	envCl->sfns.push_back(OcFunc("midPoint", "returns a point representing the bounds center", OcArg(pointCl, ptReadModify), ptNone, EnvMidPoint));

	envCl->sfns.push_back(OcFunc("contains", "", OcArg(boolCl, ptRead), ptNone, EnvContains));
	envCl->sfns.back().args.push_back(OcArg("position", pointCl, ptNone));

	envCl->sfns.push_back(OcFunc("clamp",
		"clamps the *position* coordinates to bounds, returns true if any of the *position* coordinates was clamped",
		OcArg(boolCl, ptRead), ptNone, EnvClamp));
	envCl->sfns.back().args.push_back(OcArg("position", pointCl, ptNone));

	envCl->sfns.push_back(OcFunc("simpleBounce",
		"clamps the *position* coordinates to bounds, flips the corresponding *direction* coordinates. used for simple non-physical 'bouncing' effect",
		OcArg(0, ptNone), ptNone, EnvSimpleBounce));
	envCl->sfns.back().args.push_back(OcArg("position", pointCl, ptNone));
	envCl->sfns.back().args.push_back(OcArg("direction", pointCl, ptNone));

	envCl->sfns.push_back(OcFunc("teleport", "", OcArg(0, ptNone), ptNone, EnvTeleport));
	envCl->sfns.back().args.push_back(OcArg("position", pointCl, ptNone));


	envCl->sfns.push_back(OcFunc("setPath",
		"sets the path for the log file, if *truncate* is true erases the file contents just before the start of a new run. set *truncate* to false when using in conjunction with *restart()*",
		OcArg(0, ptNone), ptNone, EnvSetPath, false, ktInter | ktAct));
	envCl->sfns.back().args.push_back(OcArg("path", textCl, ptNone));
	envCl->sfns.back().args.push_back(OcArg("truncate", boolCl, ptNone));

	envCl->sfns.push_back(OcFunc("writeln", "appends *txt* to the log file and adds a newline", OcArg(0, ptNone), ptNone, EnvWriteln, false, ktInter | ktAct));
	envCl->sfns.back().args.push_back(OcArg("txt", textCl, ptNone));

	envCl->sfns.push_back(OcFunc("write", "appends *txt* to the log file", OcArg(0, ptNone), ptNone, EnvWrite, false, ktInter | ktAct));
	envCl->sfns.back().args.push_back(OcArg("txt", textCl, ptNone));


	envCl->sfns.push_back(OcFunc("cursorActive",
		"returns true if the pointing device is active. in cinnabar that means the *Ctrl* key is pressed while dragging the mouse",
		OcArg(boolCl, ptRead), ptNone, EnvCursorActive));

	envCl->sfns.push_back(OcFunc("nearestCursorRayPoint",
		"returns the point on the cursor ray nearest to *pos*",
		OcArg(pointCl, ptRead), ptNone, EnvNearestCursorRayPoint));
	envCl->sfns.back().args.push_back(OcArg("pos", pointCl, ptRead));
}

// static funcs

OCF(EnvRestart) {
	((OcEnvironment *)v[ARG_ENV])->Restart();
}

OCF(EnvStepNo) {
	OCP(int, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->step;
}

OCF(EnvRunNo) {
	OCP(int, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->run;
}

OCF(EnvSetBoundsReal) {
	((OcEnvironment *)v[ARG_ENV])->SetBounds(OcBox(OcPoint(OCP(double, 0), OCP(double, 1), OCP(double, 2))));
}

OCF(EnvSet3DGrid) {
	((OcEnvironment *)v[ARG_ENV])->Set3DGrid(OCP(double, 0));
}



OCF(EnvRandPoint) {
	OCP(OcPoint, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->bounds.RandPoint();
}

OCF(EnvMidPoint) {
	OCP(OcPoint, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->bounds.MidPoint();
}

OCF(EnvContains) {
	OCP(bool, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->bounds.Contains(OCP(OcPoint, 0));
}

OCF(EnvClamp) {
	OCP(bool, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->bounds.Clamp(OCP(OcPoint, 0));
}

OCF(EnvSimpleBounce) {
	((OcEnvironment *)v[ARG_ENV])->bounds.SimpleBounce(OCP(OcPoint, 0), OCP(OcPoint, 1));
}

OCF(EnvTeleport) {
	((OcEnvironment *)v[ARG_ENV])->bounds.Teleport(OCP(OcPoint, 0));
}

OCF(EnvSetPath) {
	((OcEnvironment *)v[ARG_ENV])->file.SetPath(OCR(OcText, 0), OCP(bool, 1));
}

OCF(EnvWriteln) {
	((OcEnvironment *)v[ARG_ENV])->file.Writeln(OCR(OcText, 0));
}

OCF(EnvWrite) {
	((OcEnvironment *)v[ARG_ENV])->file.Write(OCR(OcText, 0));
}

OCF(EnvCursorActive) {
	OCP(bool, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->CursorActive();
}

OCF(EnvNearestCursorRayPoint) {
	OCP(OcPoint, ARG_RES) = ((OcEnvironment *)v[ARG_ENV])->NearestCursorRayPoint(OCP(OcPoint, 0));
}

//
// env struct
//

OcEnvironment::OcEnvironment() : restart(false) {
	Clear();
}

void OcEnvironment::Clear() {

	step = 0;
	run = restart ? run + 1 : 0;

	restart = false;
	cellSize = 0.0;
	(bounds = OcBox(100.0)).UpdateCellIndices(cellSize, o, n);
	file.Close();
}

void OcEnvironment::SetBounds(const OcBox &_bounds) {
	if (_bounds.IsValidBounds())
		(bounds = _bounds).UpdateCellIndices(cellSize, o, n);
}

void OcEnvironment::Set3DGrid(double _cellSize) {
	if (_cellSize >= 0.0)
		bounds.UpdateCellIndices(cellSize = _cellSize, o, n);
}

bool OcEnvironment::CursorActive() {
	return mouseRay.min != mouseRay.max;
}

OcPoint OcEnvironment::NearestCursorRayPoint(const OcPoint &p) {
	return mouseRay.NearestDiagonalPoint(p, false);
}

void OcEnvironment::Restart() {
	restart = true;
}
