#include <stdio.h>

#include "pcb.h"
#include "timer.h"

int userMain(int argc, char** argv);

int main(int argc, char** argv) {

	PCB mainPCB;
	PCB::running = &mainPCB;

	init();

	int ret = userMain(argc, argv);

	disableContextSwitch;
	printf("User main return: %d\n", ret);
	enableContextSwitch;

	restore();

	return 0;
}
