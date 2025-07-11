#include "ui.h"
#include "ctype.h"
#include "errno.h"
#include "generate.h"
#include "generateRace.h"
#include "inheritance.h"
#include "raceView.h"
#include "statUp.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

const char *const statsNames[STAT_AMOUNT] = {"Speed", "Stamina", "Power",
                                             "Guts", "Wit"};

const GradeThreshold gradeTable[] = {
    {51, "G"},    {100, "G+"},  {150, "F"},      {200, "F+"},  {250, "E"},
    {300, "E+"},  {350, "D"},   {400, "D+"},     {450, "C"},   {600, "C+"},
    {700, "B"},   {800, "B+"},  {900, "A"},      {1000, "A+"}, {1050, "S"},
    {1100, "S+"}, {1150, "SS"}, {INT_MAX, "SS+"} // catch-all
};

static const size_t gradeTableSize = sizeof(gradeTable) / sizeof(gradeTable[0]);

const char *gradeConvert(int stat) {
  for (size_t i = 0; i < gradeTableSize; ++i) {
    if (stat < gradeTable[i].threshold) {
      return gradeTable[i].grade;
    }
  }
  return "??";
}

int getConfirmation(const char *prompt) {
  char confirm[10];
  printf("%s", prompt);
  fgets(confirm, sizeof(confirm), stdin);
  if (strchr(confirm, '\n') == NULL) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }
  confirm[strcspn(confirm, "\n")] = '\0';
  for (char *c = confirm; *c; ++c) {
    *c = tolower(*c);
  }
  return (strcmp(confirm, "yes") == 0 || strcmp(confirm, "y") == 0);
}

void printGeneratedPlayerStats(int **stats, int *statsWitBonus, int avg) {
  int i;

  for (i = 0; i < STAT_AMOUNT - 1; i++) {
    printf("%s: %d (%s)\n(Base %d + %d Wit Buff)\n", statsNames[i], *stats[i],
           gradeConvert(*stats[i]), *stats[i] - statsWitBonus[i],
           statsWitBonus[i]);
  }
  printf("Wit: %d (%s)\n", *stats[4],
         gradeConvert(*stats[4])); // wit does not get a wit buff

  printf("Average: %d (%s)\n", avg, gradeConvert(avg));
}

char *enterName(void) {
  char *name = malloc(NAME_LENGTH_MAX);
  if (!name) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(1);
  }

  while (1) {
    printf("Enter a name for your Umamusume: ");
    fgets(name, NAME_LENGTH_MAX, stdin);
    if (strchr(name, '\n') == NULL) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
    }
    name[strcspn(name, "\n")] = '\0';

    if (getConfirmation("Is this name okay? (yes/yes or no/n): "))
      break;
  }

  return name;
}

void printUmaStats(const Uma *uma, const char *label) {
  printf("%s's %s:\n", uma->name, label);
  printf("Speed: %d (%s)\n", uma->stats.speed, gradeConvert(uma->stats.speed));
  printf("Stamina: %d (%s)\n", uma->stats.stamina,
         gradeConvert(uma->stats.stamina));
  printf("Power: %d (%s)\n", uma->stats.power, gradeConvert(uma->stats.power));
  printf("Guts: %d (%s)\n", uma->stats.guts, gradeConvert(uma->stats.guts));
  printf("Wit: %d (%s)\n", uma->stats.wit, gradeConvert(uma->stats.wit));
  printf("Average: %d (%s)\n\n", uma->stats.average,
         gradeConvert(uma->stats.average));
}

void playerWon(void) {
  printf("\n%s is now the champion of UmaLite! You win!\n\n", PlayerUma.name);
  printUmaStats(&PlayerUma, "Final Stats");
  if (getc(stdin))
    exit(0);
}

void playerLost(int placement) {
  printf("\n%s placed %d out of %d, ending their career.\n", PlayerUma.name,
         placement, NPC_AMOUNT + 1);
  printf("%s's dreams of becoming champion are over.\n", PlayerUma.name);

  char prompt[256];
  snprintf(
      prompt, sizeof(prompt),
      "\nDo you want to inherit %s's stats and try again? (yes/y or no/n): ",
      PlayerUma.name);

  if (getConfirmation(prompt)) {
    umaInheritance();
  } else {
    exit(0);
  }
}

const char *typeName(TrackType type) {
  switch (type) {
  case TRACK_DIRT:
    return "Dirt";
  case TRACK_TURF:
    return "Turf";
  default:
    return "Unknown";
  }
}

const char *lengthName(TrackLength length) {
  switch (length) {
  case LENGTH_SPRINT:
    return "Sprint";
  case LENGTH_MILE:
    return "Mile";
  case LENGTH_MEDIUM:
    return "Medium";
  case LENGTH_LONG:
    return "Long";
  default:
    return "Unknown";
  }
}

const char *conditionName(TrackCondition cond) {
  switch (cond) {
  case COND_FIRM:
    return "Firm";
  case COND_GOOD:
    return "Good";
  case COND_SOFT:
    return "Soft";
  case COND_WET:
    return "Wet";
  default:
    return "Unknown";
  }
}

void printRace(int index, Race r) {
  printf("[%d] %s: %s %s Race (%s)\n", index + 1, r.course->courseName,
         lengthName(r.chosenTrackLength), typeName(r.chosenTrackType),
         conditionName(r.chosenConditions));
}

void printCurrentRace(Race r) {
  printf("You chose [%s: %s %s Race (%s)]\n", r.course->courseName,
         lengthName(r.chosenTrackLength), typeName(r.chosenTrackType),
         conditionName(r.chosenConditions));
}

int getValidatedInt(const char *prompt, int min, int max) {
  int value;
  char buf[1024];
  char *endptr;
  int valid;

  do {
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin))
      return min;

    errno = 0;
    value = strtol(buf, &endptr, 10);
    valid = errno != ERANGE && endptr != buf &&
            (*endptr == '\n' || *endptr == '\0') && value >= min &&
            value <= max;

    if (!valid)
      printf("Invalid input. Please enter a number between %d and %d.\n", min,
             max);
  } while (!valid);

  return value;
}

void printStatUp(char *text, int playerPlacement, int availPoints) {
  printf(text, PlayerUma.name, playerPlacement, availPoints);
}

void displayStatOptions(void) {
  for (int i = 0; i < STAT_AMOUNT; ++i) {
    int value = 0;
    switch (i) {
    case 0:
      value = PlayerUma.stats.speed;
      break;
    case 1:
      value = PlayerUma.stats.stamina;
      break;
    case 2:
      value = PlayerUma.stats.power;
      break;
    case 3:
      value = PlayerUma.stats.guts;
      break;
    case 4:
      value = PlayerUma.stats.wit;
      break;
    }

    printf("[%d] %s (Current Stat: %d (%s))\n", i + 1, statsNames[i], value,
           gradeConvert(value));
  }
}

void printInheritedStats(const int *preBoosts, const int *boosts,
                         int **newStats, Uma oldUma) {
  printf("\n%s inherits some of %s's stats and improves their own!\n",
         PlayerUma.name, oldUma.name);

  for (size_t i = 0; i < STAT_AMOUNT; i++) {
    printf("%s: %3d (+%2d) -> %3d\n", statsNames[i], preBoosts[i], boosts[i],
           *newStats[i]);
  }
}

void renderRace(UmaRaceStats umas[]) {
  system("cls");

  for (int i = 0; i < TOTAL_UMAS; i++) {
    printf("[%2d][%-18s] ", i + 1, umas[i].uma.name);

    for (int j = 0; j < FINISH_LINE; j++) {
      if (j == umas[i].position)
        putchar(umas[i].icon);
      else
        putchar('=');
    }

    printf("|\n");
  }
}