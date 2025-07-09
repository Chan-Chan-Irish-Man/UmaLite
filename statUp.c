#include "statUp.h"
#include "race.h"
#include "stdio.h"
#include "stdlib.h"
#include "ui.h"

// ============= UTILITIES =============

int statUpAmount(int playerPlacement) {
  return SKILL_UP_POOL / playerPlacement;
}

int addPoints(int skillChoice, int amount) {
  int *statPtr = NULL;

  switch (skillChoice) {
  case SKILL_SPEED:
    statPtr = &PlayerUma.stats.speed;
    break;
  case SKILL_STAMINA:
    statPtr = &PlayerUma.stats.stamina;
    break;
  case SKILL_POWER:
    statPtr = &PlayerUma.stats.power;
    break;
  case SKILL_GUTS:
    statPtr = &PlayerUma.stats.guts;
    break;
  case SKILL_WIT:
    statPtr = &PlayerUma.stats.wit;
    break;
  default:
    return 0;
  }

  *statPtr += amount;
  return amount;
}

void applyWitToAllStats() {
  PlayerUma.stats.speed =
      applyWitBuff(PlayerUma.stats.speed, PlayerUma.stats.wit);
  PlayerUma.stats.stamina =
      applyWitBuff(PlayerUma.stats.stamina, PlayerUma.stats.wit);
  PlayerUma.stats.power =
      applyWitBuff(PlayerUma.stats.power, PlayerUma.stats.wit);
  PlayerUma.stats.guts =
      applyWitBuff(PlayerUma.stats.guts, PlayerUma.stats.wit);
}

// ============= MAIN SKILL UP FUNCTION =============

void statUp(int playerPlacement) {
  int availPoints = statUpAmount(playerPlacement);

  printStatUp("%s placed %d, therefore you can use %d points.\n",
              playerPlacement, availPoints);

  while (availPoints > 0) {
    displayStatOptions();

    int skillChoice = getValidatedInt("Select a skill to train (1-5): ", 1, 5);
    int amountChoice = getValidatedInt("Enter amount to add: ", 0, availPoints);

    availPoints -= addPoints(skillChoice, amountChoice);
    printf("%d points are left.\n", availPoints);

    applyWitToAllStats();
  }

  PlayerUma.stats.average = averageStat(
      PlayerUma.stats.speed, PlayerUma.stats.stamina, PlayerUma.stats.power,
      PlayerUma.stats.guts, PlayerUma.stats.wit);
}
