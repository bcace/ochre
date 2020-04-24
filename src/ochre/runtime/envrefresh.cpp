#include "runtime/env.h"
#include "ochre.h"



// Refreshes the run-time environment.
void OcEnv::Refresh(Ochre *ochre) {

	// only refresh env if idle or paused

	if (state != envIdle && state != envPaused)
		return;

	/*
	 * if the parse-time env is ok, cross-reference types
	 * with pools and create/delete/update pools as neccessary
	 */

	if (ochre->Parsed()) {

		OcTypeIdMap m = ochre->GetTypeIdMap();
		for (iterator p = begin(); p != end();) {

			OcTypeIdMap::iterator i = m.find(p->id);

			if (i == m.end()) // type id not found, delete pool
				p = erase(p);
			else {
				++p;
				m.erase(i);
			}
		}

		// create new pools from the remains of the type/id map

		for (OcTypeIdMap::iterator i = m.begin(); i != m.end(); ++i)
			push_back(OcPool(i->first, i->second->Name()));
	}

	/*
	 * do any updates to account for changes outside the code
	 * (changes independent of whether code is successfully parsed or not)
	 */

	// update grid threads (automatically checks if update is needed)

	for (iterator p = begin(); p != end(); ++p)
		p->Grid().UpdateThreads(threadsCount);

	/*
	 * updates for changes in code (exec code generation)
	 * do not generate if generated code is up-to-date or if there are parse errors
	 */

	if (ochre->ParseId() == generateId || !ochre->Parsed())
		return;

	// reallocate and populate pool pointers chunk

	ChkAllocate(&pls, size() * sizeof(OcPool *));

	int off = 0, idx = 0;	// todo: demoronize this
	for (iterator p = begin(); p != end(); ++p) {

		p->SetOffset(off);
		p->SetIndex(idx);
		*((OcPool **)(pls + off)) = &(*p);
		off += sizeof(OcPool *);
		++idx;
	}

	// create schedules

	setSchedule.clear();
	declSchedule.clear();
	iterSchedule.clear();
	sortSchedule.clear();

	// create schedule tasks and assign routines

	for (iterator p = begin(); p != end(); ++p) {

		// add sort task

		sortSchedule.push_back(OcTask(this, itpSort, &(*p)));

		// add routine tasks

		p->ClearRoutines();
		OcType *t = ochre->Type(p->id);

		t->UpdateOffsets(ochre);		// update var offsets BEFORE they get used in specific interaction section tasks

		for (OcSections::iterator s = t->Sections().begin(); s != t->Sections().end(); ++s) {

			const SectionDesc &desc = s->desc;

			if (desc.ignored) {
				s->SetRoutine(0);
				continue;
			}

			OcRoutine *r = 0;
			switch (desc.type) {

				case ktNoKey:
				declSchedule.push_back(OcTask(this, itpDecl, &(*p), r = p->NewRoutine(desc.beg, desc.end)));
				break;

				case ktSet:
				setSchedule.push_back(OcTask(this, itpSet, &(*p), r = p->NewRoutine(desc.beg, desc.end)));
				break;

				case ktSee:
				if (desc.var != 0)	// specific see
					iterSchedule.push_back(OcTask(this, itpSpecSee, &(*p), Pool(desc.other->Id()), desc.var->off, r = p->NewRoutine(desc.beg, desc.end)));
				else				// general see
					iterSchedule.push_back(OcTask(this, itpImplSee, &(*p), Pool(desc.other->Id()), r = p->NewRoutine(desc.beg, desc.end)));
				break;

				case ktMod:
				if (desc.var != 0)	// specific mod
					iterSchedule.push_back(OcTask(this, itpSpecMod, &(*p), Pool(desc.other->Id()), desc.var->off, r = p->NewRoutine(desc.beg, desc.end)));
				break;

				case ktAct:
				iterSchedule.push_back(OcTask(this, itpAct, &(*p), r = p->NewRoutine(desc.beg, desc.end)));
				break;

				case ktDo:
				iterSchedule.push_back(OcTask(this, itpDo, &(*p), r = p->NewRoutine(desc.beg, desc.end)));
				break;

				default:;
				break;
			}

			s->SetRoutine(r);
		}

		p->Update(t->svars, t->vars);	// update type/agent chunks
		sts[p->Index()] = p->stc;		// add pool static chunk to array
	}

	// code generation goes AFTER ALL TYPES update their var offsets

	for (Ochre::iterator t = ochre->begin(); t != ochre->end(); ++t)
		t->Generate(ochre);				// populate routines

	// sort tasks by their execution phase

	iterSchedule.sort();

	// mark generated environment as up-to-date with the last successful parse

	generateId = ochre->ParseId();
}

void OcEnv::SortPrologue() {
	for (iterator p = begin(); p != end(); ++p)
		p->Grid().SortPrologue();
}

void OcEnv::SortEpilogue() {
	for (iterator p = begin(); p != end(); ++p)
		p->Grid().SortEpilogue();
}
