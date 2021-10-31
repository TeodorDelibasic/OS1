#ifndef KERSEM_H_
#define KERSEM_H_

#include "pcb.h"
#include "list.h"

class Semaphore;

class KernelSem {

public:

	KernelSem(int init);

	int wait(Time maxTimeToWait);
	void signal();

	int val() const;

	static void timeTickAll();

	~KernelSem();

private:

	static List<KernelSem*> allKerSem;

	struct TimePCB {

		TimePCB(PCB* myPCB, Time myTime) {
			this->myPCB = myPCB;
			this->myTime = myTime;
		}

		PCB* myPCB;
		Time myTime;
	};

	int value;

	void timeTick();

	List<PCB*> waitingForSignal;
	List<TimePCB*> waitingForTime;
};

#endif
