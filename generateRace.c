#include "generateRace.h"
#include "generate.h"
#include "race.h"
#include "statUp.h"
#include "stdio.h"
#include "ui.h"

// =================== STATE ===================

int turn = 0;
int hasPlayerWonFinalRace = 0;

// =================== HELPERS ===================

Stats toStats(const Uma *uma) {
  return (Stats){.speed = uma->stats.speed,
                 .stamina = uma->stats.stamina,
                 .power = uma->stats.power,
                 .guts = uma->stats.guts,
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
    statUp(playerPlacement);
    turn++;
  }
}
