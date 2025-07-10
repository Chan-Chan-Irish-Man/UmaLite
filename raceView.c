#include "raceView.h"
#include "generate.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "windows.h"

UmaRaceStats umas[TOTAL_UMAS];

void renderRace(UmaRaceStats umas[]) {
  system("cls");

  for (int i = 0; i < TOTAL_UMAS; i++) {
    printf("[%2d][%-18s] ", i + 1, umas[i].uma.name);

    for (int j = 0; j < FINISH_LINE; j++) {
      if (j == umas[i].position)
        putchar(umas[i].icon);
      else
        putchar('=');
    }

    printf("|\n");
  }
}

int allFinished(UmaRaceStats umas[]) {
  for (int i = 0; i < TOTAL_UMAS; i++) {
    if (umas[i].position < FINISH_LINE) {
      return 0;
    }
  }
  return 1;
}

void initializeRaceUmas(int playerScore, int npcScores[]) {
  int total = TOTAL_UMAS;
  int allScores[TOTAL_UMAS];
  int speedMap[TOTAL_UMAS];

  for (int i = 0; i < NPC_AMOUNT; i++) {
    allScores[i] = npcScores[i];
  }
  allScores[NPC_AMOUNT] = playerScore;

  int indices[TOTAL_UMAS];
  for (int i = 0; i < total; i++)
    indices[i] = i;

  for (int i = 0; i < total - 1; i++) {
    for (int j = i + 1; j < total; j++) {
      if (allScores[indices[i]] < allScores[indices[j]]) {
        int temp = indices[i];
        indices[i] = indices[j];
        indices[j] = temp;
      }
    }
  }

  for (int rank = 0; rank < total; rank++) {
    int umaIndex = indices[rank];
    speedMap[umaIndex] = TOTAL_UMAS - rank;
    if (speedMap[umaIndex] < 1)
      speedMap[umaIndex] = 1;
  }

  for (int i = 0; i < NPC_AMOUNT; i++) {
    umas[i].uma = NPCUma[i];
    umas[i].icon = 'U';
    umas[i].position = 0;
    umas[i].score = speedMap[i];
    umas[i].isPlayer = 0;
  }

  umas[NPC_AMOUNT].uma = PlayerUma;
  umas[NPC_AMOUNT].icon = '@';
  umas[NPC_AMOUNT].position = 0;
  umas[NPC_AMOUNT].score = speedMap[NPC_AMOUNT];
  umas[NPC_AMOUNT].isPlayer = 1;
}

void raceView(int playerScore, int npcScores[]) {
  initializeRaceUmas(playerScore, npcScores);

  while (!allFinished(umas)) {
    for (int i = 0; i < TOTAL_UMAS; i++) {
      umas[i].position += umas[i].score;
      if (umas[i].position > FINISH_LINE)
        umas[i].position = FINISH_LINE;
    }

    renderRace(umas);
    Sleep(RACE_SPEED);
  }
}
