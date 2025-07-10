#include "generate.h"
#include "inheritance.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ui.h"

// =================== NAME POOLS ===================
static const char *umaForename[] = {
    "Top",   "Admire",  "Mejiro", "Silence", "Nice",    "Great", "King",
    "Queen", "Special", "Daiwa",  "Rice",    "Digital", "Hishi", "Mihono"};

static const char *umaSurname[] = {
    "Scarlet", "Nature", "Vodka", "Ship",   "Shower",  "Cap",     "Teio",
    "Opera",   "Flash",  "Urara", "Wonder", "Tachyon", "McQueen", "Bourbon"};

static char *generatedNames[NPC_AMOUNT];
static int generatedCount = 0;
static int retries = 0;
static const int maxRetries = 1000;

// =================== UTILITIES ===================
int isDuplicate(const char *name) {
  for (int i = 0; i < generatedCount; ++i) {
    if (strcmp(generatedNames[i], name) == 0)
      return 1;
  }
  return 0;
}

void resetGeneratedNames(void) {
  for (int i = 0; i < generatedCount; ++i) {
    free(generatedNames[i]);
    generatedNames[i] = NULL;
  }
  generatedCount = 0;
}

int statGenerate(void) { return rand() % (STAT_MAX - STAT_MIN + 1) + STAT_MIN; }

int averageStat(int s, int t, int p, int g, int w) {
  return (s + t + p + g + w) / STAT_AMOUNT;
}

double witBuff(int rawWit) {
  return (double)rawWit / (WIT_SCALING_FACTOR + rawWit);
}

int applyWitBuff(int stat, int wit) {
  int statBuff = stat * witBuff(wit);
  return stat + statBuff;
}

char *getRandomName(void) {
  char *fullName = malloc(NAME_LENGTH_MAX);
  if (!fullName) {
    perror("Memory allocation failed");
    exit(1);
  }
  snprintf(fullName, NAME_LENGTH_MAX, "%s %s", umaForename[rand() % NAME_MAX],
           umaSurname[rand() % NAME_MAX]);
  return fullName;
}

// =================== PLAYER GENERATION ===================
Uma PlayerUma;

void generatePlayerWitBonusArray(int *baseStatsArray, int *bonusArray,
                                 int wit) {
  for (int i = 0; i < STAT_AMOUNT; i++) {
    bonusArray[i] = applyWitBuff(baseStatsArray[i], wit) - baseStatsArray[i];
  }
}

void generatePlayerUma(void) {
  while (1) {
    int rawStats[STAT_AMOUNT];

    for (int i = 0; i < STAT_AMOUNT; i++) {
      rawStats[i] = statGenerate();
    }

    PlayerUma.stats.wit = rawStats[4];

    int **playerStatsArray = getStatsPointers(&PlayerUma);
    for (int i = 0; i < STAT_AMOUNT; i++) {
      *playerStatsArray[i] = applyWitBuff(rawStats[i], PlayerUma.stats.wit);
    }

    PlayerUma.stats.average = averageStat(
        PlayerUma.stats.speed, PlayerUma.stats.stamina, PlayerUma.stats.power,
        PlayerUma.stats.guts, PlayerUma.stats.wit);

    int playerWitArray[STAT_AMOUNT];
    generatePlayerWitBonusArray(rawStats, playerWitArray, PlayerUma.stats.wit);

    printGeneratedPlayerStats(playerStatsArray, playerWitArray,
                              PlayerUma.stats.average);

    if (getConfirmation("Are these stats okay? (yes/y or no/n): ")) {
      char *name = enterName();
      strcpy(PlayerUma.name, name);
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

double raceFactor(int raceNo) { return 1.0 + (raceNo - 1) * RACE_FACTOR; }

void generateNPCUma(int count, int raceNo) {
  double factor = raceFactor(raceNo);

  for (int i = 0; i < count; ++i) {
    char *name;
    do {
      name = getRandomName();
    } while (isDuplicate(name) && ++retries < maxRetries);

    if (retries >= maxRetries) {
      fprintf(stderr, "Failed to generate unique NPC name after %d tries.\n",
              maxRetries);
      exit(1);
    }

    generatedNames[generatedCount] = strdup(name);
    if (!generatedNames[generatedCount]) {
      perror("strdup failed");
      exit(1);
    }
    generatedCount++;

    Uma *npc = &NPCUma[i];
    strcpy(npc->name, name);
    free(name);

    int rawStats[STAT_AMOUNT];

    for (int i = 0; i < STAT_AMOUNT; i++) {
      rawStats[i] = statGenerate();
    }

    npc->stats.wit = rawStats[4] * factor;

    int **npcStatsArray = getStatsPointers(npc);
    for (int i = 0; i < STAT_AMOUNT; i++) {
      *npcStatsArray[i] = npcStatBuff(rawStats[i], npc->stats.wit, factor);
    }

    npc->stats.average =
        averageStat(npc->stats.speed, npc->stats.stamina, npc->stats.power,
                    npc->stats.guts, npc->stats.wit);
  }

  resetGeneratedNames();
}
