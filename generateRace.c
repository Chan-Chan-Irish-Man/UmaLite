#include "generateRace.h"
#include "ctype.h"
#include "generate.h"
#include "inheritance.h"
#include "race.h"
#include "skillUp.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

// =================== STATE ===================

int turn = 0;
int hasPlayerWonFinalRace = 0;

// =================== HELPERS ===================

void printUmaStats(const Uma *uma, const char *label) {
  printf("%s's %s:\n", uma->name, label);
  printf("Speed: %d (%s)\n", uma->speed, gradeConvert(uma->speed));
  printf("Stamina: %d (%s)\n", uma->stamina, gradeConvert(uma->stamina));
  printf("Power: %d (%s)\n", uma->power, gradeConvert(uma->power));
  printf("Guts: %d (%s)\n", uma->guts, gradeConvert(uma->guts));
  printf("Wit: %d (%s)\n", uma->wit, gradeConvert(uma->wit));
  printf("Average: %d (%s)\n\n", uma->average, gradeConvert(uma->average));
}

void playerWon() {
  printf("\n%s is now the champion of UmaLite! You win!\n\n", PlayerUma.name);
  printUmaStats(&PlayerUma, "Final Stats");
  if (getc(stdin))
    exit(0);
}

void playerLost(int placement) {
  printf("\n%s placed %d out of %d, ending their career.\n", PlayerUma.name,
         placement, NPC_AMOUNT + 1);
  printf("%s's dreams of becoming champion are over.\n", PlayerUma.name);

  char prompt[256];
  snprintf(
      prompt, sizeof(prompt),
      "\nDo you want to inherit %s's stats and try again? (yes/y or no/n): ",
      PlayerUma.name);

  if (getConfirmation(prompt)) {
    umaInheritance();
  } else {
    exit(0);
  }
}

Stats toStats(const Uma *uma) {
  return (Stats){.speed = uma->speed,
                 .stamina = uma->stamina,
                 .power = uma->power,
                 .guts = uma->guts,
                 .count = STAT_COUNT};
}

int calculateTotalScore(Stats stats, Race race) {
  return totalEffectiveness(
      typeEffectiveness(stats, race.chosenTrackType),
      lengthEffectiveness(stats, race.chosenTrackLength),
      conditionEffectiveness(stats, race.chosenConditions));
}

int findBestNpcIndex(int npcTotals[]) {
  int bestScore = 0, bestIndex = 0;
  for (int i = 0; i < NPC_AMOUNT; ++i) {
    if (npcTotals[i] > bestScore) {
      bestScore = npcTotals[i];
      bestIndex = i;
    }
  }
  return bestIndex;
}

int calculatePlayerPlacement(int playerScore, int npcTotals[]) {
  int placement = 1;
  for (int i = 0; i < NPC_AMOUNT; ++i) {
    if (playerScore < npcTotals[i])
      placement++;
  }
  return placement;
}

// =================== MAIN FUNCTION ===================

void generateRace() {
  Race availableRaces[NUM_TRACKS];
  initAvailableRaces(availableRaces, NUM_TRACKS);

  int raceChoice = getValidatedInt("Please choose a race (1-3): ", 1, 3);
  Race selectedRace = availableRaces[raceChoice - 1];
  printCurrentRace(selectedRace);

  // --- Player Score ---
  Stats playerStats = toStats(&PlayerUma);
  int playerScore = calculateTotalScore(playerStats, selectedRace);

  // --- NPC Scores ---
  int npcScores[NPC_AMOUNT];
  for (int i = 0; i < NPC_AMOUNT; ++i) {
    Stats npcStats = toStats(&NPCUma[i]);
    npcScores[i] = calculateTotalScore(npcStats, selectedRace);
  }

  // --- Placement Evaluation ---
  int playerPlacement = calculatePlayerPlacement(playerScore, npcScores);

  if (playerPlacement != 1) {
    int winnerIndex = findBestNpcIndex(npcScores);
    Uma *winner = &NPCUma[winnerIndex];

    printf("\n%s won the race.\n", winner->name);
    printUmaStats(winner, "Stats");
  } else {
    hasPlayerWonFinalRace = 1;
  }

  // --- Game Progression Rules ---
  int placementCutoff = NPC_AMOUNT + PLACEMENT_GRACE_OFFSET - turn;
  if (playerPlacement >= placementCutoff) {
    playerLost(playerPlacement);
  } else if (turnCount > NPC_AMOUNT && hasPlayerWonFinalRace) {
    playerWon();
  } else {
    skillUp(playerPlacement);
    turn++;
  }
}
