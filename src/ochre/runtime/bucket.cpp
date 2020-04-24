#include "runtime/bucket.h"
#include "runtime/agent.h"
#include "runtime/grid.h"



OcBucket::OcBucket() {}

OcBucket::~OcBucket() {
	for (iterator a = begin(); a != end(); ++a)
		delete *a;
}

void OcBucket::Clear(bool del) {

	if (del)
		for (iterator a = begin(); a != end(); ++a)
			delete *a;

	clear();
}

OcAgent *OcBucket::Pop() {
	OcAgent *a = back();
	pop_back();
	return a;
}

OcRange &OcBucket::Range(int thread) {
	return ranges[thread];
}

void OcBucket::UpdateRanges(int nt) {

	int r = size() % nt, f = (size() - r) / nt;
	int beg = 0, end = 0;

	ranges.clear();
	for (int t = 0; t < nt; ++t) {

		end += f + ((t < r) ? 1 : 0);
		ranges.push_back(OcRange(beg, end));
		beg = end;
	}
}

OcBuckets::OcBuckets() {}

OcBuckets::OcBuckets(int &threads) : std::vector<OcBucket>(threads) {}

void OcBuckets::Clear(bool del) {
	for (iterator b = begin(); b != end(); ++b)
		b->Clear(del);
}
