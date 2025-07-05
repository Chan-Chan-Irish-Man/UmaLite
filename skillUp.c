#include "skillUp.h"

int skillUpAmount(int playerPlacement)
{
  return SKILL_UP_POOL / playerPlacement;
}

int addPoints(int skillChoice, int amountChoice)
{
  switch (skillChoice)
  {
    case SKILL_SPEED: PlayerUma.speed += amountChoice; return amountChoice;
    case SKILL_STAMINA: PlayerUma.stamina += amountChoice; return amountChoice;
    case SKILL_POWER: PlayerUma.power += amountChoice; return amountChoice;
    case SKILL_GUTS: PlayerUma.guts += amountChoice; return amountChoice;
    case SKILL_WIT: PlayerUma.wit += amountChoice; return amountChoice;
    default: return 0;
  }
}

void addWitBuff()
{
  WitBuffResult newSpeed, newStamina, newPower, newGuts;

  newSpeed = applyWitBuff(PlayerUma.speed, PlayerUma.wit);
  newStamina = applyWitBuff(PlayerUma.stamina, PlayerUma.wit);
  newPower = applyWitBuff(PlayerUma.power, PlayerUma.wit);
  newGuts = applyWitBuff(PlayerUma.guts, PlayerUma.wit);

  PlayerUma.speed = newSpeed.finalValue;
  PlayerUma.stamina = newStamina.finalValue;
  PlayerUma.power = newPower.finalValue;
  PlayerUma.guts = newGuts.finalValue;


  PlayerUma.average = averageStat(PlayerUma.speed, PlayerUma.stamina,
                                  PlayerUma.power, PlayerUma.guts, PlayerUma.wit);
}

void skillUp(int playerPlacement)
{
  int availPoints = skillUpAmount(playerPlacement);

  printf("%s placed %d, therefore you can use %d points.\n",
         PlayerUma.name, playerPlacement, availPoints);

  while (availPoints > 0)
  {
    printf("[1] Speed (Current Stat: %d (%s))\n",
           PlayerUma.speed, gradeConvert(PlayerUma.speed));
    printf("[2] Stamina (Current Stat: %d (%s))\n",
           PlayerUma.stamina, gradeConvert(PlayerUma.stamina));
    printf("[3] Power (Current Stat: %d (%s))\n",
           PlayerUma.power, gradeConvert(PlayerUma.power));
    printf("[4] Guts (Current Stat: %d (%s))\n",
           PlayerUma.guts, gradeConvert(PlayerUma.guts));
    printf("[5] Wit (Current Stat: %d (%s))\n",
           PlayerUma.wit, gradeConvert(PlayerUma.wit));

    int skillChoice = getValidatedInt("Select a skill to train (1-5): ", 1, 5);

    int amountChoice = getValidatedInt("Please enter the amount you want to add: ", 0, availPoints);

    availPoints -= addPoints(skillChoice, amountChoice);

    printf("%d points are left.\n", availPoints);
  }

  addWitBuff();
}
