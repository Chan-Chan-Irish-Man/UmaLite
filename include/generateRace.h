#ifndef GENERATERACE_H
#define GENERATERACE_H

#include "generate.h"

#define PLACEMENT_GRACE_OFFSET 3
#define MIN_RACE 6
#define MAX_RACE 16

extern int playerWin;

void generateRace(Uma NPCUma[], int turn, int npcCount, int totalRaces);

#endif
