#include "ui.h"
#include "ctype.h"
#include "errno.h"
#include "generate.h"
#include "generateRace.h"
#include "inheritance.h"
#include "mood.h"
#include "raceView.h"
#include "statUp.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

extern int restart;

const char *const statsNames[STAT_AMOUNT] = {"Speed", "Stamina", "Power",
                                             "Guts", "Wit"};

const GradeThreshold gradeTable[] = {
    {G_THRESHOLD, "G"},   {G_PLUS_THRESHOLD, "G+"},
    {F_THRESHOLD, "F"},   {F_PLUS_THRESHOLD, "F+"},
    {E_THRESHOLD, "E"},   {E_PLUS_THRESHOLD, "E+"},
    {D_THRESHOLD, "D"},   {D_PLUS_THRESHOLD, "D+"},
    {C_THRESHOLD, "C"},   {C_PLUS_THRESHOLD, "C+"},
    {B_THRESHOLD, "B"},   {B_PLUS_THRESHOLD, "B+"},
    {A_THRESHOLD, "A"},   {A_PLUS_THRESHOLD, "A+"},
    {S_THRESHOLD, "S"},   {S_PLUS_THRESHOLD, "S+"},
    {SS_THRESHOLD, "SS"}, {SS_PLUS_THRESHOLD, "SS+"}};

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
  char confirm[CONFIRM_LEN];
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

void printGeneratedPlayerStats(int **stats, const int *statsWitBonus, int avg) {
  int i;

  for (i = 0; i < STAT_AMOUNT - WIT_OFFSET; i++) {
    printf("%s: %d (%s)\n(Base %d + %d Wit Buff)\n", statsNames[i], *stats[i],
           gradeConvert(*stats[i]), *stats[i] - statsWitBonus[i],
           statsWitBonus[i]);
  }
  printf("Wit: %d (%s)\n", *stats[WIT],
         gradeConvert(*stats[WIT])); // wit does not get a wit buff

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

    if (getConfirmation("Is this name okay? (yes/y or no/n): "))
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

void playerLost(int placement, int npcCount) {
  printf("\n%s placed %d out of %d, ending their career.\n", PlayerUma.name,
         placement, npcCount + 1);
  printf("%s's dreams of becoming champion are over.\n", PlayerUma.name);

  char prompt[PROMPT_SIZE];
  snprintf(
      prompt, sizeof(prompt),
      "\nDo you want to inherit %s's stats and try again? (yes/y or no/n): ",
      PlayerUma.name);

  if (getConfirmation(prompt)) {
    restart = 1;
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
  printf("[%d] %s: %s %s Race (%s)\n", index + RACE_INDEX_OFFSET,
         r.course->courseName, lengthName(r.chosenTrackLength),
         typeName(r.chosenTrackType), conditionName(r.chosenConditions));
}

void printCurrentRace(Race r) {
  printf("You chose [%s: %s %s Race (%s)]\n", r.course->courseName,
         lengthName(r.chosenTrackLength), typeName(r.chosenTrackType),
         conditionName(r.chosenConditions));
}

int getValidatedInt(const char *prompt, int min, int max) {
  int value;
  char buf[INT_INPUT_BUF_SIZE];
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

void printStatUp(const char *text, int playerPlacement, int availPoints) {
  printf(text, PlayerUma.name, playerPlacement, availPoints);
}

void displayStatOptions(void) {
  for (int i = 0; i < STAT_AMOUNT; ++i) {
    int value = 0;
    switch (i) {
    case SPEED:
      value = PlayerUma.stats.speed;
      break;
    case STAMINA:
      value = PlayerUma.stats.stamina;
      break;
    case POWER:
      value = PlayerUma.stats.power;
      break;
    case GUTS:
      value = PlayerUma.stats.guts;
      break;
    case WIT:
      value = PlayerUma.stats.wit;
      break;
    }

    printf("[%d] %s (Current Stat: %d (%s))\n", i + ARRAY_OFFSET, statsNames[i],
           value, gradeConvert(value));
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

const char *getMoodName(double moodNum) {
  if (moodNum == GREAT_MULTIPLIER) {
    return "Great";
  }
  if (moodNum == GOOD_MULTIPLIER) {
    return "Good";
  }
  if (moodNum == NORMAL_MULTIPLIER) {
    return "Normal";
  }
  if (moodNum == BAD_MULTIPLIER) {
    return "Bad";
  }
  if (moodNum == AWFUL_MULTIPLIER) {
    return "Awful";
  }

  return "Unknown";
}

void printPlayerMood(double moodNum) {
  const char *mood = getMoodName(moodNum);

  printf("%s is feeling %s (effectiveness is multiplied by %.1f).\n",
         PlayerUma.name, mood, moodNum);
}

double trackLengthRenderMultiplier(Race chosenTrack) {
  switch (chosenTrack.chosenTrackLength) {
  case LENGTH_SPRINT:
    return SPRINT_RENDER_MULT;
  case LENGTH_MILE:
    return MILE_RENDER_MULT;
  case LENGTH_MEDIUM:
    return MEDIUM_RENDER_MULT;
  case LENGTH_LONG:
    return LONG_RENDER_MULT;
  default:
    return 0.0;
  }
}

void renderRace(UmaRaceStats umas[], const int *turn, Race chosenTrack,
                int finishLine, const double umaMood[], int npcCount,
                int totalRaces) {
  system("cls");

  printf("Race [%d/%d], at %s. (%s)\n", *turn, totalRaces,
         chosenTrack.course->courseName,
         lengthName(chosenTrack.chosenTrackLength));

  int maxNameWidth = 0;
  int maxMoodWidth = 0;

  for (int i = 0; i < npcCount + 1; ++i) {
    int nameLen = strlen(umas[i].uma.name);
    if (nameLen > maxNameWidth)
      maxNameWidth = nameLen;

    const char *moodStr = getMoodName(umaMood[i]);
    int moodLen = strlen(moodStr);
    if (moodLen > maxMoodWidth)
      maxMoodWidth = moodLen;
  }

  maxNameWidth += NAME_PADDING;
  maxMoodWidth += NAME_PADDING;
  for (int i = 0; i < npcCount + PLAYER_UMA; i++) {
    const char *moodStr = getMoodName(umaMood[i]);

    printf("[%2d][%-*s|%*s] ", i + 1, maxNameWidth, umas[i].uma.name,
           maxMoodWidth, moodStr);

    for (int j = 0; j < finishLine; j++) {
      putchar(j == umas[i].position ? umas[i].icon : '=');
    }

    printf("|\n");
  }

  printPlayerMood(umaMood[npcCount]);
}
