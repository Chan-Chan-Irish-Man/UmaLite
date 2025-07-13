#include "raceView.h"
#include "generate.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "ui.h"
#include "windows.h"

UmaRaceStats umas[TOTAL_UMAS];

static void initUma(UmaRaceStats *umaSlot, const Uma *uma, char icon, int speed,
                    int isPlayer) {
  umaSlot->uma = *uma;
  umaSlot->icon = icon;
  umaSlot->position = 0;
  umaSlot->score = speed;
  umaSlot->isPlayer = isPlayer;
}

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

void sortIndicesByScore(int allScores[], int indices[]) {
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
  int range = FASTEST_UMA - SLOWEST_UMA;

  for (int rank = 0; rank < TOTAL_UMAS; rank++) {
    int umaIndex = indices[rank];
    // linearly map top rank to FASTEST_UMA, lowest rank to SLOWEST_UMA
    speedMap[umaIndex] = FASTEST_UMA - (rank * range / (TOTAL_UMAS - 1));
  }
}

void fillUmaStats(int speedMap[]) {
  for (int i = 0; i < TOTAL_UMAS; i++) {
    const Uma *u = (i == NPC_AMOUNT) ? &PlayerUma : &NPCUma[i];
    char icon = (i == NPC_AMOUNT) ? '@' : 'U';
    int isPlayer = (i == NPC_AMOUNT);
    initUma(&umas[i], u, icon, speedMap[i], isPlayer);
  }
}

void initializeRaceUmas(int playerScore, int npcScores[]) {
  int allScores[TOTAL_UMAS];

  generateScoreArray(playerScore, npcScores, allScores);

  int indices[TOTAL_UMAS];

  sortIndicesByScore(allScores, indices);

  int speedMap[TOTAL_UMAS];

  mapSpeed(indices, speedMap);

  fillUmaStats(speedMap);
}

void raceView(int playerScore, int npcScores[], int turn, const char *trackName,
              double playerMood) {
  initializeRaceUmas(playerScore, npcScores);

  while (!raceEnd(umas)) {
    for (int i = 0; i < TOTAL_UMAS; i++) {
      umas[i].position += umas[i].score;
      if (umas[i].position > FINISH_LINE)
        umas[i].position = FINISH_LINE;
    }

    renderRace(umas, turn, trackName, playerMood); // in ui.c
    Sleep(RACE_SPEED);
  }
}
