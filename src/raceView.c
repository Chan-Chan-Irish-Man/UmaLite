#include "raceView.h"
#include "generate.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "ui.h"
#include "windows.h"

UmaRaceStats umas[TOTAL_UMAS];

int raceEnd(UmaRaceStats umas[]) {
  for (int i = 0; i < TOTAL_UMAS; i++) {
    if (umas[i].position < FINISH_LINE) {
      return 0;
    }
  }
  return 1;
}

void generateScoreArray(int playerScore, int npcScores[], int allScores[]) {
  for (int i = 0; i < NPC_AMOUNT; i++) {
    allScores[i] = npcScores[i];
  }
  allScores[NPC_AMOUNT] = playerScore; // last allScore index
}

void indexScore(int allScores[], int indices[]) {
  for (int i = 0; i < TOTAL_UMAS; i++) {
    indices[i] = i;
  }

  for (int i = 0; i < TOTAL_UMAS - 1; i++) {
    for (int j = i + 1; j < TOTAL_UMAS; j++) {
      if (allScores[indices[i]] < allScores[indices[j]]) {
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
      }
    }
  }
}

void mapSpeed(int indices[], int speedMap[]) {
  for (int rank = 0; rank < TOTAL_UMAS; rank++) {
    int umaIndex = indices[rank];
    speedMap[umaIndex] = TOTAL_UMAS - rank;
    if (speedMap[umaIndex] < 1) {
      speedMap[umaIndex] = 1;
    }
  }
}

void fillUmaStats(int speedMap[]) {
  for (int i = 0; i < NPC_AMOUNT; i++) {
    umas[i].uma = NPCUma[i];
    umas[i].icon = 'U';
    umas[i].position = 0;
    umas[i].score = speedMap[i]; // so well-suited umas are faster on screen
    umas[i].isPlayer = 0;
  }

  umas[NPC_AMOUNT].uma = PlayerUma;
  umas[NPC_AMOUNT].icon = '@';
  umas[NPC_AMOUNT].position = 0;
  umas[NPC_AMOUNT].score = speedMap[NPC_AMOUNT];
  umas[NPC_AMOUNT].isPlayer = 1;
}

void initializeRaceUmas(int playerScore, int npcScores[]) {
  int allScores[TOTAL_UMAS];

  generateScoreArray(playerScore, npcScores, allScores);

  int indices[TOTAL_UMAS];

  indexScore(allScores, indices);

  int speedMap[TOTAL_UMAS];

  mapSpeed(indices, speedMap);

  fillUmaStats(speedMap);
}

void raceView(int playerScore, int npcScores[], int turn,
              const char *trackName) {
  initializeRaceUmas(playerScore, npcScores);

  while (!raceEnd(umas)) {
    for (int i = 0; i < TOTAL_UMAS; i++) {
      umas[i].position += umas[i].score;
      if (umas[i].position > FINISH_LINE)
        umas[i].position = FINISH_LINE;
    }

    renderRace(umas, turn, trackName);
    Sleep(RACE_SPEED);
  }
}
