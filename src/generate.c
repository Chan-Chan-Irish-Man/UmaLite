#include "generate.h"
#include "generateRace.h"
#include "inheritance.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ui.h"

static const char *umaForename[] = {
    "Top",   "Admire",  "Mejiro", "Silence", "Nice",    "Great", "King",
    "Queen", "Special", "Daiwa",  "Rice",    "Digital", "Hishi", "Mihono"};

static const char *umaSurname[] = {
    "Scarlet", "Nature", "Vodka", "Ship",   "Shower",  "Cap",     "Teio",
    "Opera",   "Flash",  "Urara", "Wonder", "Tachyon", "McQueen", "Bourbon"};

int randNumInRange(int min, int max) { return rand() % (max + 1 - min) + min; }

int isDuplicate(const char *name, char *generatedNames[], int npcAmount) {
  for (int i = 0; i < npcAmount; ++i) {
    if (strcmp(generatedNames[i], name) == 0)
      return 1;
  }
  return 0;
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

Uma PlayerUma;

void generatePlayerWitBonusArray(const int *baseStatsArray, int *bonusArray,
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
    PlayerUma.stats.wit = rawStats[WIT];

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

int npcStatBuff(int base, int wit, double multiplier) {
  return base * (1.0 + witBuff(wit)) * multiplier;
}

double raceFactor(int raceNo) { return 1.0 + (raceNo - 1) * RACE_FACTOR; }

Uma *generateNPCUma(int npcAmount, int raceNo) {
  Uma *NPCUma = malloc(sizeof(Uma) * npcAmount);
  if (!NPCUma) {
    perror("malloc failed for NPCUma");
    exit(EXIT_FAILURE);
  }

  char *generatedNames[npcAmount]; // for duplicate checking
  for (int i = 0; i < npcAmount; ++i) {
    generatedNames[i] = NULL;
  }

  const int maxRetries = 1000;
  double factor = raceFactor(raceNo);

  for (int i = 0; i < npcAmount; ++i) {
    char *name = NULL;
    int retries = 0;

    do {
      if (name)
        free(name);
      name = getRandomName();
    } while (isDuplicate(name, generatedNames, i) && ++retries < maxRetries);

    if (retries >= maxRetries) {
      fprintf(stderr, "Failed to generate unique NPC name after %d tries.\n",
              maxRetries);
      for (int j = 0; j < i; ++j) {
        free(generatedNames[j]);
      }
      free(NPCUma);
      exit(EXIT_FAILURE);
    }

    generatedNames[i] = strdup(name);
    if (!generatedNames[i]) {
      perror("strdup failed");
      exit(EXIT_FAILURE);
    }

    Uma *npc = &NPCUma[i];
    strcpy(npc->name, name);
    free(name);

    int rawStats[STAT_AMOUNT];
    for (int s = 0; s < STAT_AMOUNT; s++) {
      rawStats[s] = statGenerate();
    }

    npc->stats.wit = rawStats[WIT];
    int **npcStatsArray = getStatsPointers(npc);
    for (int s = 0; s < STAT_AMOUNT; s++) {
      *npcStatsArray[s] = npcStatBuff(rawStats[s], npc->stats.wit, factor);
    }

    npc->stats.average =
        averageStat(npc->stats.speed, npc->stats.stamina, npc->stats.power,
                    npc->stats.guts, npc->stats.wit);
  }

  for (int i = 0; i < npcAmount; ++i) {
    free(generatedNames[i]);
    generatedNames[i] = NULL;
  }

  return NPCUma;
}
