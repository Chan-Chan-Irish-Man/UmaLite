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

  generatePlayerUma();

  // Store original generated stats before inheritance
  int baseSpeed = PlayerUma.speed;
  int baseStamina = PlayerUma.stamina;
  int basePower = PlayerUma.power;
  int baseGuts = PlayerUma.guts;
  int baseWit = PlayerUma.wit;

  // Calculate inheritance boosts
  int boostSpeed = newGameMultiplier(OldPlayerUma.speed);
  int boostStamina = newGameMultiplier(OldPlayerUma.stamina);
  int boostPower = newGameMultiplier(OldPlayerUma.power);
  int boostGuts = newGameMultiplier(OldPlayerUma.guts);
  int boostWit = newGameMultiplier(OldPlayerUma.wit);

  // Apply inheritance
  PlayerUma.speed += boostSpeed;
  PlayerUma.stamina += boostStamina;
  PlayerUma.power += boostPower;
  PlayerUma.guts += boostGuts;
  PlayerUma.wit += boostWit;

  printf("\n%s inherits some of %s's stats and improves their own!\n",
         PlayerUma.name, OldPlayerUma.name);
  printf("Speed:   %3d (+%2d) -> %3d\n", baseSpeed, boostSpeed,
         PlayerUma.speed);
  printf("Stamina: %3d (+%2d) -> %3d\n", baseStamina, boostStamina,
         PlayerUma.stamina);
  printf("Power:   %3d (+%2d) -> %3d\n", basePower, boostPower,
         PlayerUma.power);
  printf("Guts:    %3d (+%2d) -> %3d\n", baseGuts, boostGuts, PlayerUma.guts);
  printf("Wit:     %3d (+%2d) -> %3d\n", baseWit, boostWit, PlayerUma.wit);

  generateRace();
}
