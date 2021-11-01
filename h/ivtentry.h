#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "kernelEv.h"
#include "thread.h"
#include "util.h"

#define PREPAREENTRY(ivtNum, callOld)\
void interrupt interruptEvent##ivtNum(...);\
IVTEntry ivtEntry##ivtNum(ivtNum, interruptEvent##ivtNum);\
void interrupt interruptEvent##ivtNum(...) {\
	ivtEntry##ivtNum.signal();\
    if (callOld) ivtEntry##ivtNum.oldRoutine();\
    dispatch();\
};

const int numberOfEntries = 256;

class IVTEntry {

public:

	static IVTEntry* allEntries[numberOfEntries];

	IVTEntry(IVTNo ivtNo, intPointer newRoutine);
	~IVTEntry();

	void signal();

	void setVector();
	void restoreVector();

	void callOld() { (*oldRoutine)(); }

	IVTNo ivtNo;
	intPointer newRoutine, oldRoutine;
	KernelEv* myKernelEv;
};

#endif
