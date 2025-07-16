#include "inheritance.h"
#include "generate.h"
#include "generateRace.h"
#include "stdio.h"
#include "stdlib.h"
#include "ui.h"

int newGameMultiplier(int oldStat) {
  return (int)(oldStat * INHERITANCE_MULTIPLIER);
}

int **getStatsPointers(Uma *uma) {
  static int *stats[STAT_AMOUNT];
  stats[SPEED] = &uma->stats.speed;
  stats[STAMINA] = &uma->stats.stamina;
  stats[POWER] = &uma->stats.power;
  stats[GUTS] = &uma->stats.guts;
  stats[WIT] = &uma->stats.wit;

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

void umaInheritance(void) {
  Uma OldPlayerUma = PlayerUma;

  int **oldStatsArray = getStatsPointers(&OldPlayerUma);

  generatePlayerUma();

  int **newStatsArray = getStatsPointers(&PlayerUma);

  int boostArray[STAT_AMOUNT];
  int preBoostsArray[STAT_AMOUNT];

  applyStatInheritance(oldStatsArray, newStatsArray, boostArray,
                       preBoostsArray);

  printInheritedStats(preBoostsArray, boostArray, newStatsArray, OldPlayerUma);
}
