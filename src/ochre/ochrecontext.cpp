#include "ochre.h"



bool Ochre::SelectType(int typeId) {

	for (Ochre::iterator type = begin(); type != end(); ++type)
		if (type->Id() == typeId) {
			selType = type;
			return true;
		}

	selType = end();
	return false;
}

int Ochre::SelectFirstType() {
	if (size() == 0)	// fast return so we can use this api to determine if there are any pools in the env
		return -1;
	return (selType = begin())->Id();
}

int Ochre::SelectNextType() {

	if (selType != end())
		++selType;

	return (selType != end()) ? selType->Id() : -1;
}

bool Ochre::SelectPool(int id) {

	for (OcEnv::iterator p = env.begin(); p != env.end(); ++p)
		if (p->Id() == id) {
			selPool = p;
			return true;
		}

	selPool = env.end();
	return false;
}

int Ochre::SelectFirstPool() {
	selPool = env.begin();
	return env.size() ? selPool->Id() : -1;
}

int Ochre::SelectNextPool() {

	if (selPool != env.end())
		++selPool;

	return (selPool != env.end()) ? selPool->Id() : -1;
}

const char *Ochre::GetPoolName() {
	return (selPool != env.end()) ? selPool->Name().c_str() : 0;
}

int Ochre::SelectFirst() {
	return selPool->Grid().SelectFirst();
}

int Ochre::SelectNext() {
	return selPool->Grid().SelectNext();
}

void *Ochre::GetData(int o) {
	return selPool->Grid().GetData(o);
}

void *Ochre::GetStaticData(int o) {
	return selPool->GetStaticData(o);
}

int Ochre::GetVarOffset(const char *n, const char *t, int o) {
	return selPool->GetVarOffset(n, t, o);
}

const char *Ochre::GetVarName(int i, int s) {
	return selPool->GetVarName(i, s);
}

const char *Ochre::GetVarType(int i, int s) {
	return selPool->GetVarType(i, s);
}

double Ochre::GetGrid(int *grid) {
	return env.GetGrid(grid);
}

void Ochre::GetBounds(double *bounds) {
	env.GetBounds(bounds);
}

void Ochre::SetCursorRay(double *ray) {
	env.SetCursorRay(ray);
}

//
// issues
//

int Ochre::IssueCount() {
	return (selType != end()) ? selType->Issues().size() : -1;
}

const OcIssue *Ochre::Issue(int index) {

	if (selType != end() && index < (int)selType->Issues().size())
		return &(selType->Issues()[index]);

	return 0;
}
