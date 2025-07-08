#include "inheritance.h"
#include "generate.h"
#include "generateRace.h"
#include "stdio.h"
#include "string.h"

Uma OldPlayerUma;

int newGameMultiplier(int oldStat) {
  return (int)(oldStat * INHERITANCE_MULTIPLIER);
}

void umaInheritance() {
  OldPlayerUma = PlayerUma;

  int *oldStatsArray[] = {&OldPlayerUma.stats.speed,
                          &OldPlayerUma.stats.stamina,
                          &OldPlayerUma.stats.power, &OldPlayerUma.stats.guts,
                          &OldPlayerUma.stats.wit};

  generatePlayerUma();

  int *newStatsArray[] = {&PlayerUma.stats.speed, &PlayerUma.stats.stamina,
                          &PlayerUma.stats.power, &PlayerUma.stats.guts,
                          &PlayerUma.stats.wit};

  int boostArray[STAT_AMOUNT];
  int preBoostNewStats[STAT_AMOUNT];

  const char *statsNames[] = {"Speed", "Stamina", "Power", "Guts", "Wit"};

  int boost;

  for (size_t i = 0; i < STAT_AMOUNT; i++) {
    preBoostNewStats[i] = *newStatsArray[i]; // Store original
    boost = newGameMultiplier(*oldStatsArray[i]);
    *newStatsArray[i] += boost;
    boostArray[i] = boost;
  }

  printf("\n%s inherits some of %s's stats and improves their own!\n",
         PlayerUma.name, OldPlayerUma.name);

  for (size_t i = 0; i < 5; i++) {
    printf("%s: %3d (+%2d) -> %3d\n", statsNames[i], preBoostNewStats[i],
           boostArray[i], *newStatsArray[i]);
  }

  generateNPCUma(NPC_AMOUNT, 0);
  generateRace();
}
