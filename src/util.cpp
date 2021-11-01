#include "util.h"

volatile unsigned int lockCnt = 0;
volatile unsigned int contextSwitchRequested = 0;
