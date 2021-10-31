#include <dos.h>

#include "timer.h"

#include "pcb.h"
#include "kerSem.h"
#include "util.h"
#include "SCHEDULE.H"

unsigned tsp;
unsigned tss;
unsigned tbp;

intPointer oldTimer;

Time remainingTime = 0;

void init() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;

	oldTimer = getvect(8);
	setvect(8, timer);

	enableInterrupts;
#endif

}

void tick();

void interrupt timer(...) {

	if(!contextSwitchRequested) {

		(*oldTimer)();

		tick();
		KernelSem::timeTickAll();

		remainingTime--;
	}

	if(remainingTime == 0 || contextSwitchRequested) {

		if(lockCnt == 0) {

			contextSwitchRequested = 0;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if(PCB::running->state == READY && PCB::running != PCB::getIdle())
				Scheduler::put((PCB*)PCB::running);

			PCB::running = Scheduler::get();

			if(PCB::running == 0)
				PCB::running = PCB::getIdle();

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			remainingTime = PCB::running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif

		}
		else
			contextSwitchRequested = 1;

	}
}

void restore() {

#ifndef BCC_BLOCK_IGNORE
	disableInterrupts;

	setvect(8, oldTimer);

	enableInterrupts;
#endif

}
