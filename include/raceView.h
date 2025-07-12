#ifndef RACEVIEW_H
#define RACEVIEW_H

#define TOTAL_UMAS NPC_AMOUNT + 1
#define FINISH_LINE 40
#define RACE_SPEED 300

#include "generate.h"

typedef struct {
  Uma uma;
  char icon;
  int position;
  int score;
  int isPlayer;
} UmaRaceStats;

void raceView(int playerScore, int npcScores[], int turn, const char *trackName,
              double playerMood);

#endif
