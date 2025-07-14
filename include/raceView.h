#ifndef RACEVIEW_H
#define RACEVIEW_H

#define FINISH_LINE 40
#define RACE_SPEED 300 // lower = faster rendering

#define FASTEST_UMA NPC_AMOUNT
#define SLOWEST_UMA 1

#include "generate.h"

typedef struct {
  Uma uma;
  char icon;
  int position;
  int score;
  int isPlayer;
} UmaRaceStats;

typedef struct {
  int index;
  int score;
} UmaScore;

void raceView(int playerScore, int npcScores[], int turn, const char *trackName,
              double playerMood, int npcCount, Uma NPCUma[], int totalRaces);

#endif
