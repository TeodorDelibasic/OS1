#include "thread.h"

#include "pcb.h"
#include "timer.h"
#include "util.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	disableContextSwitch;
	this->myPCB = new PCB(this, stackSize, timeSlice, PCB::wrapper);
	enableContextSwitch;
}

Thread::~Thread() {
	disableContextSwitch;
	delete this->myPCB;
	enableContextSwitch;
}

void Thread::start() {
	this->myPCB->start();
}

void Thread::waitToComplete() {
	this->myPCB->waitToComplete();
}

ID Thread::getId() {
	return this->myPCB->getId();
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}

void dispatch() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;

	contextSwitchRequested = 1;
	timer();

	enableInterrupts;
#endif

}
