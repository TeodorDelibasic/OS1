#include "pcb.h"

void idleFun() {
	while(1);
}

PCB* PCB::getIdle() {

	static PCB idlePCB(0, 1024, 1, idleFun);
	idlePCB.state = READY;

	return &idlePCB;
}
