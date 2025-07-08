#include "inheritance.h"
#include "generate.h"
#include "generateRace.h"
#include "skillUp.h"
#include "stdio.h"
#include "string.h"
#include "ui.h"

Uma OldPlayerUma;

int newGameMultiplier(int oldStat) {
  return (int)(oldStat * INHERITANCE_MULTIPLIER);
}

int **getStatsPointers(Uma *uma) {
  static int *stats[STAT_AMOUNT];
  stats[0] = &uma->stats.speed;
  stats[1] = &uma->stats.stamina;
  stats[2] = &uma->stats.power;
  stats[3] = &uma->stats.guts;
  stats[4] = &uma->stats.wit;

  return stats;
}

void applyStatInheritance(int **oldStats, int **newStats, int *boosts,
                          int *preBoosts) {
  for (size_t i = 0; i < STAT_AMOUNT; i++) {
    preBoosts[i] = *newStats[i]; // Store original
    int boost = newGameMultiplier(*oldStats[i]);
    *newStats[i] += boost;
    boosts[i] = boost;
  }
}

void umaInheritance() {
  OldPlayerUma = PlayerUma;

  int **oldStatsArray = getStatsPointers(&OldPlayerUma);

  generatePlayerUma();

  int **newStatsArray = getStatsPointers(&PlayerUma);

  int boostArray[STAT_AMOUNT];
  int preBoostsArray[STAT_AMOUNT];

  applyStatInheritance(oldStatsArray, newStatsArray, boostArray,
                       preBoostsArray);

  printInheritedStats(preBoostsArray, boostArray, newStatsArray);

  generateNPCUma(NPC_AMOUNT, 0);
  generateRace();
}
