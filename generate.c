#include "string.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdio.h"
#include "ctype.h"
#include "generate.h"

// =================== NAME POOLS ===================
static const char* umaForename[] = {
  "Top", "Admire", "Mejiro", "Silence", "Nice", "Great",
  "King", "Queen", "Special", "Daiwa", "Rice", "Digital",
  "Hishi", "Mihono"
};

static const char* umaSurname[] = {
  "Scarlet", "Nature", "Vodka", "Ship", "Shower", "Cap",
  "Teio", "Opera", "Flash", "Urara", "Wonder", "Tachyon",
  "McQueen", "Bourbon"
};

static char* generatedNames[NPC_AMOUNT];
static int generatedCount = 0;
static int retries = 0;
static const int maxRetries = 1000;

// ================== GRADE TABLE ===================

const GradeThreshold gradeTable[] = {
  {   51,  "G"   },
  {  100,  "G+"  },
  {  150,  "F"   },
  {  200,  "F+"  },
  {  250,  "E"   },
  {  300,  "E+"  },
  {  350,  "D"   },
  {  400,  "D+"  },
  {  450,  "C"   },
  {  600,  "C+"  },
  {  700,  "B"   },
  {  800,  "B+"  },
  {  900,  "A"   },
  { 1000,  "A+"  },
  { 1050,  "S"   },
  { 1100,  "S+"  },
  { 1150,  "SS"  },
  { INT_MAX, "SS+" } // catch-all
};

// =================== UTILITIES ===================
int isDuplicate(const char* name) {
  for (int i = 0; i < generatedCount; ++i) {
    if (strcmp(generatedNames[i], name) == 0) return 1;
  }
  return 0;
}

void resetGeneratedNames() {
  for (int i = 0; i < generatedCount; ++i) {
    free(generatedNames[i]);
    generatedNames[i] = NULL;
  }
  generatedCount = 0;
}

int statGenerate() {
  return rand() % (STAT_MAX - STAT_MIN + 1) + STAT_MIN;
}

int averageStat(int s, int t, int p, int g, int w) {
  return (s + t + p + g + w) / 5;
}

double witBuff(int rawWit) {
  return rawWit / (WIT_SCALING_FACTOR + rawWit);
}

WitBuffResult applyWitBuff(int stat, int wit) {
  WitBuffResult result;
  result.witBonus = stat * witBuff(wit);
  result.finalValue = stat + result.witBonus;
  return result;
}

const char* gradeConvert(int stat) {
  for (size_t i = 0; i < sizeof(gradeTable) / sizeof(gradeTable[0]); ++i) {
    if (stat < gradeTable[i].threshold) {
      return gradeTable[i].grade;
    }
  }
  return "??";
}

char* getRandomName() {
  char* fullName = malloc(50);
  if (!fullName) {
    perror("Memory allocation failed");
    exit(1);
  }
  snprintf(fullName, 50, "%s %s",
           umaForename[rand() % 12],
           umaSurname[rand() % 12]);
  return fullName;
}

char* enterName() {
  char* name = malloc(50);
  if (!name) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }

  while (1) {
    char confirm[10];
    printf("Enter a name for your Umamusume: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Is this name okay? (yes/y or no/n): ");
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';

    for (char* c = confirm; *c; ++c) *c = tolower(*c);
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) break;
  }

  return name;
}

// =================== PLAYER GENERATION ===================
Uma PlayerUma;

void generatePlayerUma() {
  while (1) {
    int rawWit = statGenerate();
    WitBuffResult speed = applyWitBuff(statGenerate(), rawWit);
    WitBuffResult stamina = applyWitBuff(statGenerate(), rawWit);
    WitBuffResult power = applyWitBuff(statGenerate(), rawWit);
    WitBuffResult guts = applyWitBuff(statGenerate(), rawWit);

    int avg = averageStat(speed.finalValue, stamina.finalValue, power.finalValue, guts.finalValue, rawWit);

    printf("Speed: %d (%s)\n(Base %d + %d Wit Buff)\n", speed.finalValue, gradeConvert(speed.finalValue), speed.finalValue - speed.witBonus, speed.witBonus);
    printf("Stamina: %d (%s)\n(Base %d + %d Wit Buff)\n", stamina.finalValue, gradeConvert(stamina.finalValue), stamina.finalValue - stamina.witBonus, stamina.witBonus);
    printf("Power: %d (%s)\n(Base %d + %d Wit Buff)\n", power.finalValue, gradeConvert(power.finalValue), power.finalValue - power.witBonus, power.witBonus);
    printf("Guts: %d (%s)\n(Base %d + %d Wit Buff)\n", guts.finalValue, gradeConvert(guts.finalValue), guts.finalValue - guts.witBonus, guts.witBonus);
    printf("Wit: %d (%s)\n", rawWit, gradeConvert(rawWit));
    printf("Average: %d (%s)\n", avg, gradeConvert(avg));

    char confirm[10];
    printf("Are these stats okay? (yes/y or no/n): ");
    fgets(confirm, sizeof(confirm), stdin);
    confirm[strcspn(confirm, "\n")] = '\0';
    for (char* c = confirm; *c; ++c) *c = tolower(*c);
    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0) {
      char* name = enterName();
      strcpy(PlayerUma.name, name);
      PlayerUma.speed = speed.finalValue;
      PlayerUma.stamina = stamina.finalValue;
      PlayerUma.power = power.finalValue;
      PlayerUma.guts = guts.finalValue;
      PlayerUma.wit = rawWit;
      PlayerUma.average = avg;
      free(name);
      break;
    }
  }
}

// =================== NPC GENERATION ===================
Uma NPCUma[NPC_AMOUNT];

int npcStatBuff(int base, int wit, double multiplier) {
  return base * (1.0 + witBuff(wit)) * multiplier;
}

double raceFactor(int raceNo) {
  return 1.0 + (raceNo - 1) * 0.3;
}

void generateNPCUma(int count, int raceNo) {
  double factor = raceFactor(raceNo);

  for (int i = 0; i < count; ++i) {
    char* name;
    do {
      name = getRandomName();
    } while (isDuplicate(name) && ++retries < maxRetries);

    if (retries >= maxRetries) {
      fprintf(stderr, "Failed to generate unique NPC name after %d tries.\n", maxRetries);
      exit(1);
    }

    generatedNames[generatedCount] = strdup(name);
    generatedCount++;

    Uma* npc = &NPCUma[i];
    strcpy(npc->name, name);
    free(name);

    npc->wit = statGenerate() * factor;
    npc->speed = npcStatBuff(statGenerate(), npc->wit, factor);
    npc->stamina = npcStatBuff(statGenerate(), npc->wit, factor);
    npc->power = npcStatBuff(statGenerate(), npc->wit, factor);
    npc->guts = npcStatBuff(statGenerate(), npc->wit, factor);
    npc->average = averageStat(npc->speed, npc->stamina, npc->power, npc->guts, npc->wit);
  }

  resetGeneratedNames();
}
