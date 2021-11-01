#ifndef TIMER_H_
#define TIMER_H_

typedef unsigned int Time;

extern unsigned tsp;
extern unsigned tss;
extern unsigned tbp;

extern Time remainingTime;

void init();
void interrupt timer(...);
void restore();

#endif
