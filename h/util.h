#ifndef UTIL_H_
#define UTIL_H_

#define nullptr 0

typedef void (*funPointer)();
typedef void interrupt (*intPointer)(...);

extern volatile unsigned int lockCnt;
extern volatile unsigned int contextSwitchRequested;

#define disableContextSwitch ++lockCnt
#define enableContextSwitch if(--lockCnt == 0 && contextSwitchRequested) dispatch()

#define disableInterrupts\
	asm {\
		pushf;\
		cli;\
	}
#define enableInterrupts\
	asm {\
		popf\
	}

#endif
