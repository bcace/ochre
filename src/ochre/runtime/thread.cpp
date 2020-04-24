#include "runtime/thread.h"
#include "runtime/task.h"
#include <time.h>
#include <iostream>



// Creates n threads and makes them wait.
void OcThreadPool::Create(int n) {

	clear();

	unsigned int seed = time(0);

	for (int i = 0; i < n; ++i)
		push_back(OcThread(i, seed));

	for (int i = 0; i < n; ++i) {
		pthread_mutex_init(&at(i).blockWorker, 0);
		pthread_mutex_init(&at(i).awaitTask, 0);
		pthread_mutex_lock(&at(i).awaitTask);
		pthread_create(&at(i).ptr, 0, ThreadFunc, &at(i));
	}
}

// Tells all threads to stop and joins them.
void OcThreadPool::Destroy() {

	for (int i = 0; i < (int)size(); ++i) {
		at(i).stop = true;
		pthread_mutex_unlock(&at(i).awaitTask);
	}

	Join();

	for (int i = 0; i < (int)size(); ++i) {
		pthread_join(at(i).ptr, 0);
		pthread_mutex_destroy(&at(i).blockWorker);
		pthread_mutex_destroy(&at(i).awaitTask);
	}

	clear();
}

// Joins any running threads, then starts them
// on a new task.
void OcThreadPool::Run(OcTask *task, CHUNK *lit, CHUNK *jmp, CHUNK *pls, CHUNK **sts, unsigned int pools, void *env) {

	Join();

	if (task->phase & itpSerial) {
		pthread_mutex_lock(&at(0).blockWorker);
		at(0).frame.AllocateChunks(task, lit, jmp, pls, sts, pools, env);
		at(0).task = task;
		at(0).stop = false;
		pthread_mutex_unlock(&at(0).awaitTask);
	}
	else {
		for (int i = 0; i < (int)size(); ++i) {
			pthread_mutex_lock(&at(i).blockWorker);
			at(i).frame.AllocateChunks(task, lit, jmp, pls, sts, pools, env);
			at(i).task = task;
			at(i).stop = false;
			pthread_mutex_unlock(&at(i).awaitTask);
		}
	}

	Join();
}

// Joins any running threads, then starts them
// on a new task.
void OcThreadPool::RunSort(OcTask *task) {

	Join();

	for (int i = 0; i < (int)size(); ++i) {
		pthread_mutex_lock(&at(i).blockWorker);
		at(i).task = task;
		at(i).stop = false;
		pthread_mutex_unlock(&at(i).awaitTask);
	}

	Join();
}

// Waits until all threads are done.
void OcThreadPool::Join() {

	for (iterator t = begin(); t != end(); ++t) {
		pthread_mutex_lock(&t->blockWorker);
		pthread_mutex_unlock(&t->blockWorker);
		t->frame.DeleteChunks(t->task);
		t->task = 0;	// reset task so subsequent deletes behave correctly
	}
}

//
// debug funcs
//

void OcThreadPool::Step() {
	for (iterator t = begin(); t != end(); ++t)
		if (t->suspended)
			t->stepToNextInstruction = true;
}

//
// thread
//

OcThread::OcThread(int _index, unsigned int _seed) :
	index(_index),
	seed(_seed + _index),
	stop(false),
	stepToNextInstruction(false),
	suspended(false),
	task(0) {}
