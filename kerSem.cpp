#include "kerSem.h"
#include "SCHEDULE.H"
#include "pcb.h"
#include "util.h"

List<KernelSem*> KernelSem::allKerSem;

KernelSem::KernelSem(int init) {
	this->value = init;

	disableContextSwitch;

	allKerSem.pushBack(this);

	enableContextSwitch;
}

KernelSem::~KernelSem() {

	disableContextSwitch;

	allKerSem.removeData(this);

	enableContextSwitch;
}

int KernelSem::wait(Time maxTimeToWait) {

	disableContextSwitch;

	if(--this->value >= 0) {

		enableContextSwitch;

		return 1;
	}
	else {

		if(maxTimeToWait > 0)
			this->waitingForTime.pushBack(new TimePCB((PCB*)PCB::running, maxTimeToWait));
		else
			this->waitingForSignal.pushBack((PCB*)PCB::running);

		PCB::running->state = BLOCKED;

		enableContextSwitch;

		dispatch();
	}

	return PCB::running->unblockType;
}

void KernelSem::signal() {

	disableContextSwitch;

	if(++this->value <= 0) {

		PCB* unblocked;

		if(!waitingForSignal.empty())
			unblocked = waitingForSignal.popFront();
		else if(!waitingForTime.empty())
			unblocked = waitingForTime.popFront()->myPCB;

		if(unblocked != 0) {
			unblocked->unblockType = 1;
			unblocked->state = READY;

			Scheduler::put(unblocked);
		}
	}

	enableContextSwitch;
}

int KernelSem::val() const {
	return this->value;
}

void KernelSem::timeTickAll() {
	disableContextSwitch;

	for(allKerSem.setCurr(); allKerSem.hasCurr(); allKerSem.moveCurr()) {
		allKerSem.getCurrData()->timeTick();
	}

	enableContextSwitch;
}

void KernelSem::timeTick() {

	disableContextSwitch;

	for(waitingForTime.setCurr(); waitingForTime.hasCurr(); ) {
		TimePCB* timePCB = waitingForTime.getCurrData();

		if(--timePCB->myTime == 0) {
			PCB* unblocked = timePCB->myPCB;

			unblocked->unblockType = 0;
			unblocked->state = READY;

			Scheduler::put(unblocked);

			waitingForTime.deleteCurr();

			delete timePCB;

			this->value = this->value + 1;
		}
		else
			waitingForTime.moveCurr();
	}

	enableContextSwitch;
}
