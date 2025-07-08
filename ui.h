#ifndef UI_H
#define UI_H

#include "generate.h"
#include "race.h"

extern const char *const statsNames[STAT_AMOUNT];

// used in generate.c
int getConfirmation(const char *prompt);
const char *gradeConvert(int stat);
char *enterName();

// used in generateRace.c
void printUmaStats(const Uma *uma, const char *label);
void playerWon();
void playerLost(int placement);

// used in race.c
const char *typeName(TrackType type);
const char *lengthName(TrackLength length);
const char *conditionName(TrackCondition cond);
void printRace(int index, Race r);
void printCurrentRace(Race r);
int getValidatedInt(const char *prompt, int min, int max);

// used in statUp.c
void printStatUp(char *text, int playerPlacement, int availPoints);
void displayStatOptions();

// used in inheritance.c
void printInheritedStats(const int *preBoosts, const int *boosts,
                         int **newStats);

#endif
