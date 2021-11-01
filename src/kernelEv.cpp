#include "kernelEv.h"
#include "ivtentry.h"
#include "pcb.h"
#include "schedule.h"
#include <stdio.h>

KernelEv::KernelEv(IVTNo ivtNo) {
	this->owner = (PCB*)PCB::running;
	this->ivtNo = ivtNo;
	this->value = 0;

	if(IVTEntry::allEntries[ivtNo]) {
		IVTEntry::allEntries[ivtNo]->myKernelEv = this;
		IVTEntry::allEntries[ivtNo]->setVector();
	}
}

KernelEv::~KernelEv() {
	if(IVTEntry::allEntries[ivtNo]) {
		IVTEntry::allEntries[ivtNo]->myKernelEv = 0;
		IVTEntry::allEntries[ivtNo]->restoreVector();
	}
}

void KernelEv::wait() {

	disableContextSwitch;

	if(PCB::running == this->owner) {

		if(this->value-- == 0) {

			PCB::running->state = BLOCKED;

			enableContextSwitch;

			dispatch();
		}
		else
			enableContextSwitch;
	}
	else
		enableContextSwitch;
}

void KernelEv::signal() {

	disableContextSwitch;

	if(this->value == -1) {

		this->owner->state = READY;

		Scheduler::put(this->owner);
	}

	if(this->value < 1)
		this->value++;

	enableContextSwitch;
}







