#include "ivtEntry.h"
#include "util.h"

#include <dos.h>
#include <stdio.h>

IVTEntry* IVTEntry::allEntries[numberOfEntries] = {nullptr};

IVTEntry::IVTEntry(IVTNo ivtNo, intPointer newRoutine) {
	allEntries[this->ivtNo = ivtNo] = this;

	this->newRoutine = newRoutine;
	this->oldRoutine = nullptr;

	this->myKernelEv = nullptr;
}

IVTEntry::~IVTEntry() {
	allEntries[this->ivtNo] = nullptr;
}

void IVTEntry::signal() {
	if(this->myKernelEv)
		this->myKernelEv->signal();
}

void IVTEntry::setVector() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts

	this->oldRoutine = getvect(this->ivtNo);
	setvect(this->ivtNo, this->newRoutine);

	enableInterrupts
#endif

}

void IVTEntry::restoreVector() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts

	setvect(this->ivtNo, this->oldRoutine);

	enableInterrupts
#endif

}
