#include "raceView.h"
#include "generate.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "ui.h"
#include "windows.h"

static void initUma(UmaRaceStats *umaSlot, const Uma *uma, char icon, int speed,
                    int isPlayer) {
  umaSlot->uma = *uma;
  umaSlot->icon = icon;
  umaSlot->position = 0;
  umaSlot->score = speed;
  umaSlot->isPlayer = isPlayer;
}

int raceEnd(UmaRaceStats umas[], int totalUmas, int finishLine) {
  for (int i = 0; i < totalUmas; i++) {
    if (umas[i].position < finishLine) {
      return 0;
    }
  }
  return 1;
}

void generateScoreArray(int playerScore, int npcScores[], int allScores[],
                        int totalUmas) {
  for (int i = 0; i < totalUmas - 1; i++) {
    allScores[i] = npcScores[i];
  }
  allScores[totalUmas - 1] = playerScore; // Player is last in the array
}

int compareUmaScores(const void *a, const void *b) {
  const UmaScore *ua = (const UmaScore *)a;
  const UmaScore *ub = (const UmaScore *)b;
  return ub->score - ua->score; // Descending
}

void sortIndicesByScore(int allScores[], int indices[], int totalUmas) {
  UmaScore umaScr[totalUmas];

  for (int i = 0; i < totalUmas; i++) {
    umaScr[i].index = i;
    umaScr[i].score = allScores[i];
  }

  qsort(umaScr, totalUmas, sizeof(UmaScore), compareUmaScores);

  for (int i = 0; i < totalUmas; i++) {
    indices[i] = umaScr[i].index;
  }
}

void mapSpeed(int indices[], int speedMap[], int totalUmas) {
  int fastestUma = totalUmas;
  int range = fastestUma - SLOWEST_UMA;

  for (int rank = 0; rank < totalUmas; rank++) {
    int umaIndex = indices[rank];
    // Linearly map top rank to fastestUma
    speedMap[umaIndex] = fastestUma - (rank * range / (totalUmas - 1));
  }
}

void fillUmaStats(int speedMap[], int totalUmas, UmaRaceStats umas[],
                  Uma NPCUma[]) {
  for (int i = 0; i < totalUmas; i++) {
    const Uma *u = (i == totalUmas - 1) ? &PlayerUma : &NPCUma[i];
    char icon = (i == totalUmas - 1) ? '@' : 'U';
    int isPlayer = (i == totalUmas - 1);
    initUma(&umas[i], u, icon, speedMap[i], isPlayer);
  }
}

void initializeRaceUmas(int playerScore, int npcScores[], int totalUmas,
                        UmaRaceStats umas[], Uma NPCUma[]) {
  int allScores[totalUmas];
  int indices[totalUmas];
  int speedMap[totalUmas];

  generateScoreArray(playerScore, npcScores, allScores, totalUmas);
  sortIndicesByScore(allScores, indices, totalUmas);
  mapSpeed(indices, speedMap, totalUmas);
  fillUmaStats(speedMap, totalUmas, umas, NPCUma);
}

void raceView(int playerScore, int npcScores[], int turn, Race chosenTrack,
              double umaMood[], int npcCount, Uma NPCUma[], int totalRaces) {
  const int totalUmas = npcCount + 1;
  UmaRaceStats umas[totalUmas];

  int finishLine = BASE_TRACK_LENGTH * trackLengthRenderMultiplier(chosenTrack);

  initializeRaceUmas(playerScore, npcScores, totalUmas, umas, NPCUma);

  while (!raceEnd(umas, totalUmas, finishLine)) {
    for (int i = 0; i < totalUmas; i++) {
      umas[i].position += umas[i].score;
      if (umas[i].position > finishLine) {
        umas[i].position = finishLine;
      }
    }

    renderRace(umas, turn, chosenTrack, finishLine, umaMood, npcCount,
               totalRaces); // Provided by ui.c
    Sleep(RACE_SPEED);
  }
}
