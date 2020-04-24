#include "runtime/frame.h"
#include "runtime/agent.h"
#include "runtime/instruction.h"
#include "runtime/task.h"
#include <iostream>



void OcFrame::MapArgs(OcIns *ins) {
	for (int a = 0; a < ins->argNo; ++a)
		v[ins->argReg[a]] = (void *)(c[ins->argChk[a]] + ins->argOff[a]);
}

void OcFrame::AllocateChunks(OcTask *task, CHUNK *lit, CHUNK *jmp, CHUNK *pls, CHUNK **sts, unsigned int poolCount, void *env) {

	c[fcThread] = thr;
	c[fcNext] = nxt;

	if (task->r0) {
		c[fcRes] = 0;
		ChkAllocate(&c[fcRes], task->r0->resScheme.Size());	// todo: allocate once to absolute max, and just apply destructors at DeleteChunks()
		c[fcLocal] = 0;
		ChkAllocate(&c[fcLocal], task->r0->locScheme.Size());
	}

	c[fcLit] = lit;
	c[fcJump] = jmp;
	c[fcPools] = pls;

	for (unsigned int p = 0; p < poolCount; ++p)
		c[fcStatic + p] = sts[p];

	v[ARG_ENV] = env;
}

void OcFrame::DeleteChunks(OcTask *task) {
	if (task && task->r0) {
		ChkDelete(&c[fcRes], task->r0->resScheme);
		ChkDelete(&c[fcLocal], task->r0->locScheme);
	}
}
