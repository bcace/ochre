#ifndef memmanagement_h
#define memmanagement_h

#include "runtime/chunk.h"
#include <vector>
#include <map>

#define OCD(ocd_name) void ocd_name(void *v)					// destructor, releases resources
#define OCC(occ_name) void occ_name(void *f, void *t)			// copy constructor, executes deep copy

typedef void (*CTOR)(void *);
typedef void (*COPY)(void *, void *);



class OcVars;

struct CopyElement
{
	int fOff, tOff, size;

	CopyElement(int _fOff, int _tOff, int _size) :
		fOff(_fOff), tOff(_tOff), size(_size) {}
};

class CopyElements : public std::vector<CopyElement>
{
public:
	void Apply(CHUNK *f, CHUNK *t);
};

struct CtorElement
{
	int off;
	CTOR ctor;

	CtorElement(int _off, CTOR _ctor) :
		off(_off), ctor(_ctor) {}
};

class CtorElements : public std::vector<CtorElement>
{
public:
	void Apply(CHUNK *c) {
		for (iterator e = begin(); e != end(); ++e)
			e->ctor(c + e->off);
	}
};



class CopySchema
{
public:
	bool modified;	// specifies if chunk must be reallocated and/or shuffled

	CopySchema(OcVars &, OcVars &);

	void Apply(CHUNK *, CHUNK *);

private:
	CopyElements cpy;
	CtorElements des;
};



class OcClass;

class MemScheme
{
public:
	MemScheme(int _ini = 0);

	int Size() const;
	void Clear();

	int Add(int);
	int Add(OcClass *);
	void ApplyDestructors(CHUNK *);

private:
	CtorElements des;
	int ini, size;
};

#endif
