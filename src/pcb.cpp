#include "pcb.h"
#include "SCHEDULE.H"
#include "util.h"

#include <dos.h>

volatile PCB* PCB::running = 0;
List<PCB*> PCB::allPCB;

ID PCB::idCnt = 0;

PCB::PCB() {
	this->stack = nullptr;
	this->bp = this->sp = this->ss = 0;
	this->timeSlice = defaultTimeSlice;
	this->myThread = nullptr;
	this->state = READY;
	this->unblockType = -1;

	disableContextSwitch;

	this->id = idCnt++;

	allPCB.pushBack(this);

	enableContextSwitch;
}

PCB::PCB(Thread* owner, StackSize stackSize, Time timeSlice, funPointer body) {
	stackSize = (stackSize < minStackSize ? minStackSize : stackSize);
	stackSize = (stackSize > maxStackSize ? maxStackSize : stackSize);

	stackSize = stackSize / sizeof(unsigned);

	disableContextSwitch;
	this->stack = new unsigned[stackSize];
	enableContextSwitch;

	this->stack[stackSize - 1] = 0x200;

#ifndef BCC_BLOCK_IGNORE
	this->stack[stackSize - 2] = FP_SEG(body);
	this->stack[stackSize - 3] = FP_OFF(body);

	this->sp = FP_OFF(stack + stackSize - 12);
	this->ss = FP_SEG(stack + stackSize - 12);
#endif

	this->bp = this->sp;

	disableContextSwitch;
	this->id = idCnt++;
	allPCB.pushBack(this);
	enableContextSwitch;

	this->timeSlice = timeSlice;
	this->myThread = owner;
	this->state = CREATED;
	this->unblockType = -1;
}

PCB::~PCB() {
	disableContextSwitch;
	if(this->stack != 0) {
		delete[] this->stack;
		this->stack = 0;
	}
	PCB::allPCB.removeData(this);
	enableContextSwitch;
}

void PCB::start() {
	disableContextSwitch;
	if(this->state == CREATED) {
		this->state = READY;
		Scheduler::put(this);
	}
	enableContextSwitch;
}

void PCB::waitToComplete() {
	disableContextSwitch;
	if(running != this && this->state != CREATED &&
			this->state != FINISHED && this != PCB::getIdle()) {

		this->blockedOnMe.pushBack((PCB*)running);
		running->state = BLOCKED;
		enableContextSwitch;

		dispatch();
	}
	else enableContextSwitch;
}

ID PCB::getId() {
	return this->id;
}

ID PCB::getRunningId() {
	return running->id;
}

Thread* PCB::getThreadById(ID id) {
	disableContextSwitch;
	for(allPCB.setCurr(); allPCB.hasCurr(); allPCB.moveCurr()) {
		if(allPCB.getCurrData()->id == id) {
			enableContextSwitch;
			return allPCB.getCurrData()->myThread;
		}
	}
	enableContextSwitch;

	return nullptr;
}

void PCB::wrapper() {
	running->myThread->run();

	disableContextSwitch;
	while(!((PCB*)running)->blockedOnMe.empty()) {
		PCB* blocked = ((PCB*)running)->blockedOnMe.popFront();
		blocked->state = READY;
		Scheduler::put(blocked);
	}
	running->state = FINISHED;
	enableContextSwitch;

	dispatch();
}
