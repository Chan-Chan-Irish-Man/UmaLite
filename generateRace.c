#include "generateRace.h"

int turn = 0;
int playerWin;

void playerWon()
{
  printf("%s is now the champion of UmaLite! You win!\n", PlayerUma.name);
  printf("%s's Final Stats.\n", PlayerUma.name);
  printf("Speed: %d (%s)\n", PlayerUma.speed, gradeConvert(PlayerUma.speed));
  printf("Stamina: %d (%s)\n", PlayerUma.stamina, gradeConvert(PlayerUma.stamina));
  printf("Power: %d (%s)\n", PlayerUma.power, gradeConvert(PlayerUma.power));
  printf("Guts: %d (%s)\n", PlayerUma.guts, gradeConvert(PlayerUma.guts));
  printf("Wit: %d (%s)\n", PlayerUma.wit, gradeConvert(PlayerUma.wit));
  printf("Average: %d (%s)\n", PlayerUma.average, gradeConvert(PlayerUma.average));

  exit(0);
}

void playerLost(int playerPlacement)
{
  printf("%s placed %d out of %d, ending their career.\n", PlayerUma.name, playerPlacement, NPC_AMOUNT+1);
  printf("%s's dreams of becoming champion are over.", PlayerUma.name);
  exit(0);
}

void generateRace()
{
  int playerPlacement = 1;
  playerWin = 0;

  Race availableRaces[NUM_TRACKS];

  initAvailableRaces(availableRaces, NUM_TRACKS); 

  Race currentRace = availableRaces[getValidatedInt("Please choose a race (1-3): ", 1, 3) - 1];

  printCurrentRace(currentRace);

  Stats playerStats = {PlayerUma.speed, PlayerUma.stamina, 
    PlayerUma.power, PlayerUma.guts, STAT_COUNT};

  int playerTotal = totalEffectiveness(
    typeEffectiveness(playerStats, currentRace.chosenTrackType),
    lengthEffectiveness(playerStats, currentRace.chosenTrackLength),
    conditionEffectiveness(playerStats, currentRace.chosenConditions)
  );

  Stats npcStats[NPC_AMOUNT];

  int npcTotal[NPC_AMOUNT];

  for (int i = 0; i < NPC_AMOUNT; ++i)
  {
    npcStats[i] = (Stats){NPCUma[i].speed, NPCUma[i].stamina,
    NPCUma[i].power, NPCUma[i].guts, STAT_COUNT};

    npcTotal[i] = totalEffectiveness(
      typeEffectiveness(npcStats[i], currentRace.chosenTrackType),
      lengthEffectiveness(npcStats[i], currentRace.chosenTrackLength),
      conditionEffectiveness(npcStats[i], currentRace.chosenConditions)
    );
  }

  for (int i = 0; i < NPC_AMOUNT; ++i)
  {
    if (playerTotal < npcTotal[i])
    {
      playerPlacement++;
    }
  }

  if (playerPlacement != 1)
  {
    int bestNPCTotal = 0;
    int bestNPCIndex = 0;

    for (int i = 0; i < NPC_AMOUNT; ++i)
    {
      if (bestNPCTotal < npcTotal[i])
      {
        bestNPCTotal = npcTotal[i];
        bestNPCIndex = i;
      }
    }

    printf("%s won the race.\n", NPCUma[bestNPCIndex].name);

    printf("%s's Speed: %d (%s).\n", NPCUma[bestNPCIndex].name,
           NPCUma[bestNPCIndex].speed, gradeConvert(NPCUma[bestNPCIndex].speed));

    printf("%s's Stamina: %d (%s).\n", NPCUma[bestNPCIndex].name,
           NPCUma[bestNPCIndex].stamina, gradeConvert(NPCUma[bestNPCIndex].stamina));

    printf("%s's Power: %d (%s).\n", NPCUma[bestNPCIndex].name,
           NPCUma[bestNPCIndex].power, gradeConvert(NPCUma[bestNPCIndex].power));

    printf("%s's Guts: %d (%s).\n", NPCUma[bestNPCIndex].name,
           NPCUma[bestNPCIndex].guts, gradeConvert(NPCUma[bestNPCIndex].guts));

    printf("%s's Wit: %d (%s).\n", NPCUma[bestNPCIndex].name,
           NPCUma[bestNPCIndex].wit, gradeConvert(NPCUma[bestNPCIndex].wit));

    printf("%s's Average: %d (%s).\n", NPCUma[bestNPCIndex].name,
           NPCUma[bestNPCIndex].average, gradeConvert(NPCUma[bestNPCIndex].average));
  }
  else
  {
    playerWin = 1; // so a final race win in championship wins championship
  }

  // if player places last and is first turn, continue
  // if player places last and is second turn, end game
  // if player places second last and is third turn, end game, etc.
  int placementThreshold = NPC_AMOUNT + PLACEMENT_GRACE_OFFSET - turn;
  if (playerPlacement >= placementThreshold)
  {
    playerLost(playerPlacement);
  }
  else if (turnCount > NPC_AMOUNT && playerWin == 1)
  {
    playerWon();
  }
  else
  {
    skillUp(playerPlacement);
    turn++;
  }
}
