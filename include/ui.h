#ifndef UI_H
#define UI_H

#include "generate.h"
#include "race.h"
#include "raceView.h"

extern const char *const statsNames[STAT_AMOUNT];

// used in generate.c
int getConfirmation(const char *prompt);
const char *gradeConvert(int stat);
void printGeneratedPlayerStats(int **stats, int *statsWitBonus, int avg);
char *enterName(void);

// used in generateRace.c
void printUmaStats(const Uma *uma, const char *label);
void printPlayerMood(double moodNum);
void playerWon(void);
void playerLost(int placement, int npcCount);

// used in race.c
const char *typeName(TrackType type);
const char *lengthName(TrackLength length);
const char *conditionName(TrackCondition cond);
void printRace(int index, Race r);
void printCurrentRace(Race r);
int getValidatedInt(const char *prompt, int min, int max);

// used in statUp.c
void printStatUp(char *text, int playerPlacement, int availPoints);
void displayStatOptions(void);

// used in inheritance.c
void printInheritedStats(const int *preBoosts, const int *boosts,
                         int **newStats, Uma oldUma);

// used in raceView.c
void renderRace(UmaRaceStats umas[], int turn, const char *trackName,
                double playerMood, int npcCount, int totalRaces);

#endif
