#ifndef UMA_H
#define UMA_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "ctype.h"
#include "math.h"
#include "errno.h"

#define STAT_MIN 50
#define STAT_MAX 200
#define NAME_MIN 0
#define NAME_MAX 11
#define WIT_SCALING_FACTOR 1200.0
#define NPC_AMOUNT 11

typedef struct GradeThreshold 
{
  int threshold;
  const char* grade;
} GradeThreshold;

typedef struct Uma
{
	char name[50];
	int speed;
	int stamina;
	int power;
	int guts;
	int wit;
	int average;
} Uma;

typedef struct WitBuffResult
{
  int witBonus;
  int finalValue;
} WitBuffResult;

extern int turnCount;

char* gradeConvert(int stat);

extern Uma PlayerUma;
extern Uma NPCUma[NPC_AMOUNT];

int averageStat(int rawSpeed, int rawStamina,
                int rawPower, int rawGuts, int rawWit);

double witBuff(int rawWit);
WitBuffResult applyWitBuff(int rawStat, int rawWit);

void generatePlayerUma();
void generateNPCUma(int amount, int raceNo);

extern char* generatedNames[NPC_AMOUNT];
extern int generatedCount;

void resetGeneratedNames();

#endif
