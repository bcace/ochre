#include "ochre.h"
#include <iostream>



bool Ochre::Run(int typeId, int pos) {
	return env.Run(this, typeId, pos);
}

bool Ochre::Stop() {
	return env.Stop();
}

bool Ochre::Pause() {
	return env.Pause();
}

bool Ochre::Resume() {
	return env.Resume(this);
}

bool Ochre::Reset() {
	return env.Reset();
}

int Ochre::GetState() {
	return (int)env.state;
}

bool Ochre::SetThreadCount(int threads) {
	return env.SetThreadCount(threads);
}

// following functions are debug functions
// should only be used when state is running
// and if all threads are suspended

void Ochre::Step() {
//	env.Step();
}

int Ochre::ThreadCount() {
	return (int)env.ThreadCount();
}

int Ochre::BreakType() {
	return env.BreakType();
}

int Ochre::BreakPosition() {
	return (int)env.BreakPosition();
}
