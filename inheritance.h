#ifndef INHERITANCE_H
#define INHERITANCE_H

#include "generate.h"

// =================== CONFIG CONSTANTS ===================

#define INHERITANCE_MULTIPLIER .1f

// =================== GLOBAL STATE ===================

extern Uma OldPlayerUma;

// =================== FUNCTIONS ===================

int **getStatsPointers(Uma *uma);
void umaInheritance(void);

#endif
