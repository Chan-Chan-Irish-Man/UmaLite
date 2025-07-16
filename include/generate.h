#ifndef GENERATE_H
#define GENERATE_H

#define STAT_MIN 50
#define STAT_MAX 200
#define STAT_AMOUNT 5
#define NAME_MIN 0
#define NAME_MAX 13
#define NAME_LENGTH_MAX 50
#define MIN_NPC 5
#define MAX_NPC 15
#define WIT_SCALING_FACTOR 1200.0
#define RACE_FACTOR .3f

typedef struct {
  int threshold;
  const char *grade;
} GradeThreshold;

typedef struct {
  int speed, stamina, power, guts, wit, average;
} UmaStats;

typedef struct {
  char name[50];
  UmaStats stats;
} Uma;

typedef struct {
  int witBonus, finalValue;
} WitBuffResult;

extern int turnCount;

extern Uma PlayerUma;

enum Stat { SPEED, STAMINA, POWER, GUTS, WIT };

// Uma Generation
int randNumInRange(int min, int max);
void generatePlayerUma(void);
Uma *generateNPCUma(int amount, int raceNo);

// Stat Calculations
int averageStat(int speed, int stamina, int power, int guts, int wit);
double witBuff(int wit);
int applyWitBuff(int stat, int wit);

// NPC name tracking
void resetGeneratedNames(char *generatedNames[], int npcAmount);

#endif // UMA_H
