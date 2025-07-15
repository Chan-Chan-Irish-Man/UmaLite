#include "generateRace.h"
#include "generate.h"
#include "mood.h"
#include "race.h"
#include "raceView.h"
#include "statUp.h"
#include "stdio.h"
#include "ui.h"

int hasPlayerWonFinalRace = 0;

Stats toStats(const Uma *uma) {
  return (Stats){.speed = uma->stats.speed,
                 .stamina = uma->stats.stamina,
                 .power = uma->stats.power,
                 .guts = uma->stats.guts,
                 .count = STAT_COUNT};
}

int calculateTotalScore(Stats stats, Race race, double mood) {
  return totalEffectiveness(
             typeEffectiveness(stats, race.chosenTrackType),
             lengthEffectiveness(stats, race.chosenTrackLength),
             conditionEffectiveness(stats, race.chosenConditions)) *
         mood;
}

int findBestNpcIndex(int npcTotals[], int npcCount) {
  int bestScore = 0, bestIndex = 0;
  for (int i = 0; i < npcCount; ++i) {
    if (npcTotals[i] > bestScore) {
      bestScore = npcTotals[i];
      bestIndex = i;
    }
  }
  return bestIndex;
}

int calculatePlayerPlacement(int playerScore, int npcTotals[], int npcCount) {
  int placement = 1;
  for (int i = 0; i < npcCount; ++i) {
    if (playerScore < npcTotals[i])
      placement++;
  }
  return placement;
}

void generateRace(Uma NPCUma[], int turn, int npcCount, int totalRaces,
                  int placementCutoff) {
  Race availableRaces[NUM_TRACKS];
  initAvailableRaces(availableRaces, NUM_TRACKS);

  int raceChoice = getValidatedInt("Please choose a race (1-3): ", 1, 3);
  Race selectedRace = availableRaces[raceChoice - 1];
  printCurrentRace(selectedRace);

  Stats playerStats = toStats(&PlayerUma);
  double playerMood = moodMultiplier();
  int playerScore = calculateTotalScore(playerStats, selectedRace, playerMood);

  double umaMood[npcCount];

  int npcScores[npcCount];
  for (int i = 0; i < npcCount; ++i) {
    Stats npcStats = toStats(&NPCUma[i]);
    double npcMood = moodMultiplier();

    npcScores[i] = calculateTotalScore(npcStats, selectedRace, npcMood);
    umaMood[i] = npcMood;
  }

  umaMood[npcCount] = playerMood;

  raceView(playerScore, npcScores, turn, selectedRace.course->courseName,
           umaMood, npcCount, NPCUma, totalRaces);

  int playerPlacement =
      calculatePlayerPlacement(playerScore, npcScores, npcCount);

  if (playerPlacement != 1) {
    int winnerIndex = findBestNpcIndex(npcScores, npcCount);
    Uma *winner = &NPCUma[winnerIndex];

    printf("\n%s won the race.\n", winner->name);
    printUmaStats(winner, "Stats");
  } else {
    hasPlayerWonFinalRace = 1;
  }

  if (playerPlacement >= placementCutoff) {
    playerLost(playerPlacement, npcCount);
  } else if (turn > npcCount && hasPlayerWonFinalRace) {
    playerWon();
  } else {
    statUp(playerPlacement);
    turn++;
  }
}
