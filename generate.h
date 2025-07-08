#ifndef GENERATE_H
#define GENERATE_H

// =================== CONFIG CONSTANTS ===================

#define STAT_MIN 50
#define STAT_MAX 200
#define STAT_AMOUNT 5
#define NAME_MIN 0
#define NAME_MAX 13
#define NAME_LENGTH_MAX 50
#define NPC_AMOUNT 11
#define WIT_SCALING_FACTOR 1200.0
#define RACE_FACTOR .3f

// =================== STRUCTURES ===================

typedef struct {
  int threshold;
  const char *grade;
} GradeThreshold;

typedef struct {
  int speed;
  int stamina;
  int power;
  int guts;
  int wit;
  int average;
} UmaStats;

typedef struct {
  char name[50];
  UmaStats stats;
} Uma;

typedef struct {
  int witBonus;
  int finalValue;
} WitBuffResult;

// =================== GLOBAL STATE ===================

extern int turnCount;

extern Uma PlayerUma;
extern Uma NPCUma[NPC_AMOUNT];

// =================== FUNCTIONS ===================

// Uma Generation
void generatePlayerUma();
void generateNPCUma(int amount, int raceNo);

// Stat Calculations
int averageStat(int speed, int stamina, int power, int guts, int wit);
double witBuff(int wit);
WitBuffResult applyWitBuff(int stat, int wit);

// NPC name tracking
void resetGeneratedNames();

#endif // UMA_H
