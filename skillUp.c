#include "skillUp.h"
#include "race.h"
#include "stdio.h"
#include "stdlib.h"

static const char *skillNames[] = {"Speed", "Stamina", "Power", "Guts", "Wit"};

// ============= UTILITIES =============

int skillUpAmount(int playerPlacement) {
  return SKILL_UP_POOL / playerPlacement;
}

int addPoints(int skillChoice, int amount) {
  int *statPtr = NULL;

  switch (skillChoice) {
  case SKILL_SPEED:
    statPtr = &PlayerUma.speed;
    break;
  case SKILL_STAMINA:
    statPtr = &PlayerUma.stamina;
    break;
  case SKILL_POWER:
    statPtr = &PlayerUma.power;
    break;
  case SKILL_GUTS:
    statPtr = &PlayerUma.guts;
    break;
  case SKILL_WIT:
    statPtr = &PlayerUma.wit;
    break;
  default:
    return 0;
  }

  *statPtr += amount;
  return amount;
}

void applyWitToAllSkills() {
  PlayerUma.speed = applyWitBuff(PlayerUma.speed, PlayerUma.wit).finalValue;
  PlayerUma.stamina = applyWitBuff(PlayerUma.stamina, PlayerUma.wit).finalValue;
  PlayerUma.power = applyWitBuff(PlayerUma.power, PlayerUma.wit).finalValue;
  PlayerUma.guts = applyWitBuff(PlayerUma.guts, PlayerUma.wit).finalValue;
}

void displaySkillOptions() {
  for (int i = 0; i < 5; ++i) {
    int value = 0;
    switch (i) {
    case 0:
      value = PlayerUma.speed;
      break;
    case 1:
      value = PlayerUma.stamina;
      break;
    case 2:
      value = PlayerUma.power;
      break;
    case 3:
      value = PlayerUma.guts;
      break;
    case 4:
      value = PlayerUma.wit;
      break;
    }

    printf("[%d] %s (Current Stat: %d (%s))\n", i + 1, skillNames[i], value,
           gradeConvert(value));
  }
}

// ============= MAIN SKILL UP FUNCTION =============

void skillUp(int playerPlacement) {
  int availPoints = skillUpAmount(playerPlacement);

  printf("%s placed %d, therefore you can use %d points.\n", PlayerUma.name,
         playerPlacement, availPoints);

  while (availPoints > 0) {
    displaySkillOptions();

    int skillChoice = getValidatedInt("Select a skill to train (1-5): ", 1, 5);
    int amountChoice = getValidatedInt("Enter amount to add: ", 0, availPoints);

    availPoints -= addPoints(skillChoice, amountChoice);
    printf("%d points are left.\n", availPoints);

    applyWitToAllSkills();
  }

  PlayerUma.average =
      averageStat(PlayerUma.speed, PlayerUma.stamina, PlayerUma.power,
                  PlayerUma.guts, PlayerUma.wit);
}
