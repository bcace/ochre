#include "runtime/chunk.h"
#include "descriptors/descriptors.h"
#include <cstring>



void ChkAllocate(CHUNK **c, int l) {
	if (*c)
		delete[] *c;

	*c = new CHUNK[l];
	memset(*c, 0, l);
}

void ChkAppendValue(CHUNK **c, int o, int s, void *n) {
	CHUNK *v = new CHUNK[o + s];

	if (*c) {

		// shallow copy old data

		if (o)
			memcpy(v, *c, o);

		// delete original

		delete[] *c;
	}

	// copy value

	if (s)
		memcpy(v + o, n, s);

	*c = v;
}

void ChkReallocateAndCopy(CHUNK **c, int l, int o, CopySchema *s) {
	CHUNK *v = new CHUNK[l];
	memset(v, 0, l);

	if (*c) {

		// copy kernel data

		if (o)
			memcpy(v, *c, o);

		// copy user data

		if (s)
			s->Apply(*c, v);

		// delete original

		delete[] *c;
	}

	*c = v;
}

void ChkDelete(CHUNK **c, MemScheme &s) {

	if (*c) {
		s.ApplyDestructors(*c);
		delete[] *c;
		*c = 0;
	}
}

void ChkDelete(CHUNK **c) {

	if (*c) {
		delete[] *c;
		*c = 0;
	}
}

void ChkClear(CHUNK **c, int o, int l) {
	memset((*c) + o, 0, l);
}
