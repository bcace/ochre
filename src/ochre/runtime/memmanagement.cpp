#include "runtime/memmanagement.h"
#include "descriptors/class.h"
#include <string.h>



CopySchema::CopySchema(OcVars &old, OcVars &cur) : modified(false) {

	unsigned int hits = 0;
	for (OcVars::iterator o = old.begin(); o != old.end(); ++o) {

		const OcVar *c = cur.Contains(o->name);
		if (c) {

			bool canCopy = false;

			if (o->cl != c->cl)		// if type changed
				modified = true;
			else					// if type remained the same
				canCopy = true;

			if (o->off != c->off)	// if variable moved
				modified = true;

			if (canCopy)
				cpy.push_back(CopyElement(o->off, c->off, c->cl->size));

			++hits;
		}
		else {	// variable removed, add destructor to scheme

			if (o->cl->destr)
				des.push_back(CtorElement(o->off, o->cl->destr));

			modified = true;
		}
	}

	// check if any variables added

	if (hits < cur.size())
		modified = true;
}

void CopySchema::Apply(CHUNK *f, CHUNK *t) {
	cpy.Apply(f, t);
	des.Apply(f);
}

void CopyElements::Apply(CHUNK *f, CHUNK *t) {
	for (iterator e = begin(); e != end(); ++e)
		memcpy(t + e->tOff, f + e->fOff, e->size);
}

//
// mem
//

MemScheme::MemScheme(int _ini) :
	ini(_ini), size(ini) {}

int MemScheme::Size() const {
	return size;
}

void MemScheme::Clear() {
	des.clear();
	size = ini;
}

// Adds a new mem element to scheme and returns its offset.
int MemScheme::Add(int s) {

	int off = size;
	size += s;
	return off;
}

// Adds a new mem element to scheme and returns its offset.
int MemScheme::Add(OcClass *cl) {

	if (cl && cl->destr)
		des.push_back(CtorElement(size, cl->destr));

	int off = size;
	size += cl->size;
	return off;
}

void MemScheme::ApplyDestructors(CHUNK *c) {
	des.Apply(c);
}
