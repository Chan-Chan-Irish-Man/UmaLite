/* race.c
 *
 * Has a collection of racecourses (aim for 3 now), each containing info:
 *
 * 0 name (immutable string) 
 * 0 track types (dirt and turf, some racecourses might only have one)
 * 0 track length (sprint, mile, medium, long)
 * 
 * 0 function randomly chooses between the available track types and track length:
 * - track length:
 *   + sprint (speed and power+++)
 *   + mile (speed and power++, stamina+)
 *   + medium (speed and power+, stamina++, guts+)
 *   + long (stamina+++, guts++)
 *
 *   wit can serve as a multiplier for each skill, where low wit increases
 *   	each skill a little while high wit increases each skill a lot	
 *
 * Another function randomly chooses a racecourse and then randomly chooses
 * 	 the race conditions:
 * - firm (speed+++)
 * - good (speed++, power+)
 * - soft (speed+, power++)
 * - wet (power+++)
 *
 * The player will be able to choose between three different racecourses
 * each turn which show the conditions their uma would race in, allowing
 * certain builds to succeed while others to fail.
 */

#include "race.h"
#include "stdlib.h"
#include "stdio.h"

Racecourse TRACK_LIST[] = {
  {
    .courseName = "Sunny Downs",
    .supportedTrackTypes = TRACK_TURF,
    .supportedTrackLengths = LENGTH_MEDIUM | LENGTH_LONG,
    .supportedConditions = COND_FIRM | COND_GOOD
  },
  {
    .courseName = "Rainy Meadows",
    .supportedTrackTypes = TRACK_DIRT,
    .supportedTrackLengths = LENGTH_SPRINT | LENGTH_MILE, 
    .supportedConditions = COND_SOFT | COND_WET
  },
  {
    .courseName = "AllWeather Park",
    .supportedTrackTypes = TRACK_DIRT | TRACK_TURF,
    .supportedTrackLengths = LENGTH_SPRINT | LENGTH_MILE | LENGTH_MEDIUM | LENGTH_LONG,
    .supportedConditions = COND_FIRM | COND_GOOD | COND_SOFT | COND_WET
  }
};

const int NUM_TRACKS = sizeof(TRACK_LIST) / sizeof(Racecourse);

TrackType pickRandomType(int supportedTrackTypes)
{
  TrackType options[2];
  int count = 0;

  if (supportedTrackTypes & TRACK_DIRT) options[count++] = TRACK_DIRT;
  if (supportedTrackTypes & TRACK_TURF) options[count++] = TRACK_TURF;
  if (count == 0) return 0;

  return options[rand() % count];
}

TrackLength pickRandomLength(int supportedTrackLengths)
{
  TrackLength options[4];
  int count = 0;

  if (supportedTrackLengths & LENGTH_SPRINT) options[count++] = LENGTH_SPRINT;
  if (supportedTrackLengths & LENGTH_MILE) options[count++] = LENGTH_MILE;
  if (supportedTrackLengths & LENGTH_MEDIUM) options[count++] = LENGTH_MEDIUM;
  if (supportedTrackLengths & LENGTH_LONG) options[count++] = LENGTH_LONG;
  if (count == 0) return 0;

  return options[rand() % count];
}

TrackCondition pickRandomCondition(int supportedConditions)
{
  TrackCondition options[4];
  int count = 0;

  if (supportedConditions & COND_FIRM) options[count++] = COND_FIRM;
  if (supportedConditions & COND_GOOD) options[count++] = COND_GOOD;
  if (supportedConditions & COND_SOFT) options[count++] = COND_SOFT;
  if (supportedConditions & COND_WET) options[count++] = COND_WET;
  if (count == 0) return 0;

  return options[rand() % count];
}

const char* typeName(TrackType type)
{
  switch (type)
  {
    case TRACK_DIRT: return "Dirt";
    case TRACK_TURF: return "Turf";
    default: return "Unknown";
  }
}

const char* lengthName(TrackLength length)
{
  switch (length)
  {
    case LENGTH_SPRINT: return "Sprint";
    case LENGTH_MILE: return "Mile";
    case LENGTH_MEDIUM: return "Medium";
    case LENGTH_LONG: return "Long";
    default: return "Unknown";
  }
}

const char* conditionName(TrackCondition cond)
{
  switch (cond)
  {
    case COND_FIRM: return "Firm";
    case COND_GOOD: return "Good";
    case COND_SOFT: return "Soft";
    case COND_WET: return "Wet";
    default: return "Unknown";
  }
}

Race createRace(const Racecourse* course)
{
  Race race;

  race.course = course;

  race.chosenTrackType = pickRandomType(course->supportedTrackTypes);
  race.chosenTrackLength = pickRandomLength(course->supportedTrackLengths);
  race.chosenConditions = pickRandomCondition(course->supportedConditions);

  return race;
}

void printRace(int index, Race r)
{
  printf("[%d] %s: %s %s Race (%s)\n", index + 1,
         r.course->courseName,
         lengthName(r.chosenTrackLength),
         typeName(r.chosenTrackType),
         conditionName(r.chosenConditions));
}

void initAvailableRaces(Race availableRaces[], int count)
{
  for (int i = 0; i < count; ++i) {
    availableRaces[i] = createRace(&TRACK_LIST[i]);
    printRace(i, availableRaces[i]);
  }  
}

int getValidatedInt(const char* prompt, int min, int max)
{
  int value;
  char buf[1024];
  char* endptr;
  int valid;

  do {
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return min;
    errno = 0;
    value = strtol(buf, &endptr, 10);
    valid = errno != ERANGE && endptr != buf && (*endptr == '\n' || *endptr == '\0') &&
            value >= min && value <= max;

    if (!valid)
      printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
  } while (!valid);

  return value;
}

void printCurrentRace(Race r)
{
  printf("You chose [%s: %s %s Race (%s)]\n",
         r.course->courseName,
         lengthName(r.chosenTrackLength),
         typeName(r.chosenTrackType),
         conditionName(r.chosenConditions));
}

int integerAverage(int a, int b, int c, int d, int count)
{
  return (a + b + c + d) / count;
}

int typeEffectiveness(Stats stats, TrackType type)
{
  switch (type)
  {
    case TRACK_DIRT: return integerAverage(stats.speed, 
                                           stats.stamina, 
                                           (stats.power * DIRT_POWER_BUFF), 
                                           (stats.guts * DIRT_GUTS_BUFF), 
                                           STAT_COUNT);
    case TRACK_TURF: return integerAverage((stats.speed * TURF_SPEED_BUFF), 
                                           (stats.stamina * TURF_STAMINA_BUFF), 
                                           stats.power, 
                                           stats.guts, 
                                           STAT_COUNT);
    default: return 0;
  }
}

int lengthEffectiveness(Stats stats, TrackLength length)
{
  switch (length)
  {
    case LENGTH_SPRINT: return integerAverage((stats.speed * SPRINT_SPEED_BUFF),
                                              stats.stamina,
                                              (stats.power * SPRINT_POWER_BUFF),
                                              stats.guts,
                                              STAT_COUNT);
    case LENGTH_MILE: return integerAverage((stats.speed * MILE_SPEED_BUFF),
                                            (stats.stamina * MILE_STAMINA_BUFF),
                                            (stats.power * MILE_POWER_BUFF),
                                            (stats.guts * MILE_GUTS_BUFF),
                                            STAT_COUNT);
    case LENGTH_MEDIUM: return integerAverage((stats.speed * MEDIUM_SPEED_BUFF),
                                              (stats.stamina * MEDIUM_STAMINA_BUFF),
                                              (stats.power * MEDIUM_POWER_BUFF),
                                              (stats.guts * MEDIUM_GUTS_BUFF),
                                              STAT_COUNT);
    case LENGTH_LONG: return integerAverage(stats.speed,
                                            (stats.stamina * LONG_STAMINA_BUFF),
                                            stats.power,
                                            (stats.guts * LONG_GUTS_BUFF),
                                            STAT_COUNT);
    default: return 0;
  }
}

int conditionEffectiveness(Stats stats, TrackCondition condition)
{
  switch (condition)
  {
    case COND_FIRM: return integerAverage((stats.speed * FIRM_SPEED_BUFF),
                                          stats.stamina,
                                          stats.power,
                                          (stats.guts * FIRM_GUTS_BUFF),
                                          STAT_COUNT);
    case COND_GOOD: return integerAverage((stats.speed * GOOD_SPEED_BUFF),
                                          stats.stamina,
                                          stats.power,
                                          (stats.guts * GOOD_GUTS_BUFF),
                                          STAT_COUNT);
    case COND_SOFT: return integerAverage(stats.speed,
                                          (stats.stamina * SOFT_STAMINA_BUFF),
                                          (stats.power * SOFT_POWER_BUFF),
                                          stats.guts,
                                          STAT_COUNT);
    case COND_WET: return integerAverage(stats.speed,
                                         (stats.stamina * WET_STAMINA_BUFF),
                                         (stats.power * WET_POWER_BUFF),
                                         stats.guts,
                                         STAT_COUNT);
    default: return 0;
  }
}

int totalEffectiveness(int typeEffect, int lengthEffect, int conditionEffect)
{
  return typeEffect + lengthEffect + conditionEffect;
}

