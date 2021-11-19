#ifndef PCB_H_
#define PCB_H_

#include "list.h"

class Thread;

enum State { CREATED, READY, BLOCKED, FINISHED };

void idleFun();

const StackSize minStackSize = 512;
const StackSize maxStackSize = 65535;

class PCB {

public:
	static volatile PCB* running;
	static List<PCB*> allPCB;

	static ID idCnt;

	static PCB* getIdle();

	PCB();
	PCB(Thread* owner, StackSize stackSize, Time timeSlice, funPointer body);

	~PCB();

	void start();
	void waitToComplete();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	static void wrapper();

private:
	Thread* myThread;

	ID id;

	unsigned bp, sp, ss;
	unsigned* stack;

	Time timeSlice;
	State state;
	List<PCB*> blockedOnMe;
	int unblockType;
};

#endif
