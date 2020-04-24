#ifndef thread_h
#define thread_h

#include "runtime/frame.h"
#include <pthread.h>
#include <vector>



class OcTask;

struct OcThread
{
	int index;
	unsigned seed;
	bool stop;
	bool stepToNextInstruction;
	bool suspended;					// only used to notify env thread that the thread is suspended
	pthread_t ptr;
	pthread_mutex_t blockWorker;	// used to block threadPool while joining threads (wait)
	pthread_mutex_t awaitTask;		// used to block each thread to wait for another task
	OcFrame frame;
	OcTask *task;

	OcThread(int, unsigned int);
};

void *ThreadFunc(void *);



class OcThreadPool : public std::vector<OcThread>
{
public:
	void Create(int);
	void Destroy();
	void Run(OcTask *, CHUNK *, CHUNK *, CHUNK *, CHUNK **, unsigned int, void *);
	void RunSort(OcTask *);

	void Step();

private:
	void Join();
};

#endif
