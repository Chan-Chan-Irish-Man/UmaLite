#ifndef RACEVIEW_H
#define RACEVIEW_H

#define BASE_TRACK_LENGTH 40
#define RACE_SPEED 300 // lower = faster rendering

#define FASTEST_UMA NPC_AMOUNT
#define SLOWEST_UMA 1

#include "generate.h"
#include "race.h"

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

void raceView(int playerScore, int npcScores[], int turn, Race chosenTrack,
              double umaMood[], int npcCount, Uma NPCUma[], int totalRaces);

#endif
