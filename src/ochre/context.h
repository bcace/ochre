#ifndef context_h
#define context_h



class OcAgent;
class OcFrame;

struct RuntimeContext
{
	OcAgent *agent;
	OcFrame *frame;

	RuntimeContext() : agent(0), frame(0) {}
};

#endif
