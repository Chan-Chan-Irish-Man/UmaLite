#ifndef UI_H
#define UI_H

#include "limits.h"

#include "generate.h"
#include "race.h"
#include "raceView.h"

#define MOODNAME_LENGTH_MAX 10

#define CONFIRM_LEN 10
#define MOOD_LEN 10
#define NAME_PADDING 2
#define INPUT_BUF_SIZE 1024
#define INT_INPUT_BUF_SIZE 10
#define ARRAY_OFFSET 1
#define PLAYER_UMA 1
#define RACE_INDEX_OFFSET 1
#define PROMPT_SIZE 256
#define WIT_OFFSET 1

#define SPRINT_RENDER_MULT 0.8
#define MILE_RENDER_MULT 1.0
#define MEDIUM_RENDER_MULT 1.2
#define LONG_RENDER_MULT 1.4

enum GradeThresholds {
  G_THRESHOLD = 51,
  G_PLUS_THRESHOLD = 100,
  F_THRESHOLD = 150,
  F_PLUS_THRESHOLD = 200,
  E_THRESHOLD = 250,
  E_PLUS_THRESHOLD = 300,
  D_THRESHOLD = 350,
  D_PLUS_THRESHOLD = 400,
  C_THRESHOLD = 450,
  C_PLUS_THRESHOLD = 600,
  B_THRESHOLD = 700,
  B_PLUS_THRESHOLD = 800,
  A_THRESHOLD = 900,
  A_PLUS_THRESHOLD = 1000,
  S_THRESHOLD = 1050,
  S_PLUS_THRESHOLD = 1100,
  SS_THRESHOLD = 1150,
  SS_PLUS_THRESHOLD = INT_MAX // catch-all
};

extern const char *const statsNames[STAT_AMOUNT];

// used in generate.c
int getConfirmation(const char *prompt);
const char *gradeConvert(int stat);
void printGeneratedPlayerStats(int **stats, const int *statsWitBonus, int avg);
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
void printStatUp(const char *text, int playerPlacement, int availPoints);
void displayStatOptions(void);

// used in inheritance.c
void printInheritedStats(const int *preBoosts, const int *boosts,
                         int **newStats, Uma oldUma);

// used in raceView.c
double trackLengthRenderMultiplier(Race chosenTrack);
void renderRace(UmaRaceStats umas[], const int *turn, Race chosenTrack,
                int finishLine, const double umaMood[], int npcCount,
                int totalRaces);

#endif
