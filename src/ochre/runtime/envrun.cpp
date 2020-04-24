#include "runtime/env.h"
#include "runtime/instruction.h"
#include <windows.h>



void *EnvRun(void *arg) {

	OcEnv *e = (OcEnv *)arg;

	// execute simulation steps

	while (!e->stop) {		// stop thread

		if (e->run) {		// pause thread

			if (!e->setDone) {

				e->ResetEnvironment();

				// execute 'set' routines

				OcSchedule &setSched = e->SetSchedule();
				for (OcSchedule::iterator t = setSched.begin(); t != setSched.end() && !e->stop; ++t) {

					OcRoutine *r = t->r0;
					if (r->size()) {
						e->Worker().Run(&(*t), r->lit, r->jmp, e->pls, e->sts, e->size(), &e->env);
					}
				}

				e->setDone = true;
			}
			else {

				// update grids

				e->UpdateGrids();

				// execute 'decl' routines

				OcSchedule &d = e->DeclSchedule();
				for (OcSchedule::iterator t = d.begin(); t != d.end() && !e->stop; ++t) {

					OcRoutine *r = t->r0;
					if (r->size()) {
						e->Worker().Run(&(*t), r->lit, r->jmp, e->pls, e->sts, e->size(), &e->env);
					}
				}

				// execute sorting tasks

				e->SortPrologue();

				OcSchedule &sortSched = e->SortSchedule();
				for (OcSchedule::iterator t = sortSched.begin(); t != sortSched.end() && !e->stop; ++t) {
					e->Worker().RunSort(&(*t));
				}

				e->SortEpilogue();

				// execute regular schedule tasks

				OcSchedule &sched = e->Schedule();
				for (OcSchedule::iterator t = sched.begin(); t != sched.end() && !e->stop; ++t) {

					OcRoutine *r = t->r0;
					if (r->size()) {
						e->Worker().Run(&(*t), r->lit, r->jmp, e->pls, e->sts, e->size(), &e->env);
					}
				}

				++e->env.step;
			}

			// restart

			e->Restart();
		}
		else {
			e->asleep = true;
			Sleep(1);	// pause wait
		}
	}

	return 0;
}
