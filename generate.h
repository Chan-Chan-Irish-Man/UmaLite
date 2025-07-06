#ifndef GENERATE_H
#define GENERATE_H

#include "limits.h" // for INT_MAX

// =================== CONFIG CONSTANTS ===================

#define STAT_MIN 50
#define STAT_MAX 200
#define NAME_MIN 0
#define NAME_MAX 11
#define NPC_AMOUNT 11
#define WIT_SCALING_FACTOR 1200.0

// =================== STRUCTURES ===================

typedef struct {
    int threshold;
    const char* grade;
} GradeThreshold;

typedef struct {
    char name[50];
    int speed;
    int stamina;
    int power;
    int guts;
    int wit;
    int average;
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

// Grade Conversion
const char* gradeConvert(int stat);

// NPC name tracking
void resetGeneratedNames();

#endif // UMA_H
