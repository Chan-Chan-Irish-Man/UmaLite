#include "race.h"
#include "stdlib.h"
#include "ui.h"

Racecourse TRACK_LIST[] = {
    {.courseName = "Sunny Downs",
     .supportedTrackTypes = TRACK_TURF,
     .supportedTrackLengths = LENGTH_MEDIUM | LENGTH_LONG,
     .supportedConditions = COND_FIRM | COND_GOOD},
    {.courseName = "Rainy Meadows",
     .supportedTrackTypes = TRACK_DIRT,
     .supportedTrackLengths = LENGTH_SPRINT | LENGTH_MILE,
     .supportedConditions = COND_SOFT | COND_WET},
    {.courseName = "AllWeather Park",
     .supportedTrackTypes = TRACK_DIRT | TRACK_TURF,
     .supportedTrackLengths =
         LENGTH_SPRINT | LENGTH_MILE | LENGTH_MEDIUM | LENGTH_LONG,
     .supportedConditions = COND_FIRM | COND_GOOD | COND_SOFT | COND_WET}};

const int NUM_TRACKS = sizeof(TRACK_LIST) / sizeof(Racecourse);

static int pickRandomFromBitmask(int mask, int options[], int maxOptions) {
  int count = 0;

  for (int i = 0; i < maxOptions; ++i) {
    if (mask & (1 << i)) {
      options[count++] = (1 << i);
    }
  }

  return count > 0 ? options[rand() % count] : 0;
}

TrackType pickRandomType(int supportedTrackTypes) {
  int options[2];
  return (TrackType)pickRandomFromBitmask(supportedTrackTypes, options, 2);
}

TrackLength pickRandomLength(int supportedTrackLengths) {
  int options[4];
  return (TrackLength)pickRandomFromBitmask(supportedTrackLengths, options, 4);
}

TrackCondition pickRandomCondition(int supportedConditions) {
  int options[4];
  return (TrackCondition)pickRandomFromBitmask(supportedConditions, options, 4);
}

Race createRace(const Racecourse *course) {
  return (Race){
      .course = course,
      .chosenTrackType = pickRandomType(course->supportedTrackTypes),
      .chosenTrackLength = pickRandomLength(course->supportedTrackLengths),
      .chosenConditions = pickRandomCondition(course->supportedConditions)};
}

void initAvailableRaces(Race availableRaces[], int count) {
  for (int i = 0; i < count; ++i) {
    availableRaces[i] = createRace(&TRACK_LIST[i]);
    printRace(i, availableRaces[i]);
  }
}

int integerAverage(int a, int b, int c, int d, int count) {
  return (a + b + c + d) / count;
}

int typeEffectiveness(Stats s, TrackType t) {
  switch (t) {
  case TRACK_DIRT:
    return integerAverage(s.speed, s.stamina, s.power * DIRT_POWER_BUFF,
                          s.guts * DIRT_GUTS_BUFF, STAT_COUNT);
  case TRACK_TURF:
    return integerAverage(s.speed * TURF_SPEED_BUFF,
                          s.stamina * TURF_STAMINA_BUFF, s.power, s.guts,
                          STAT_COUNT);
  default:
    return 0;
  }
}

int lengthEffectiveness(Stats s, TrackLength l) {
  switch (l) {
  case LENGTH_SPRINT:
    return integerAverage(s.speed * SPRINT_SPEED_BUFF, s.stamina,
                          s.power * SPRINT_POWER_BUFF, s.guts, STAT_COUNT);
  case LENGTH_MILE:
    return integerAverage(
        s.speed * MILE_SPEED_BUFF, s.stamina * MILE_STAMINA_BUFF,
        s.power * MILE_POWER_BUFF, s.guts * MILE_GUTS_BUFF, STAT_COUNT);
  case LENGTH_MEDIUM:
    return integerAverage(
        s.speed * MEDIUM_SPEED_BUFF, s.stamina * MEDIUM_STAMINA_BUFF,
        s.power * MEDIUM_POWER_BUFF, s.guts * MEDIUM_GUTS_BUFF, STAT_COUNT);
  case LENGTH_LONG:
    return integerAverage(s.speed, s.stamina * LONG_STAMINA_BUFF, s.power,
                          s.guts * LONG_GUTS_BUFF, STAT_COUNT);
  default:
    return 0;
  }
}

int conditionEffectiveness(Stats s, TrackCondition c) {
  switch (c) {
  case COND_FIRM:
    return integerAverage(s.speed * FIRM_SPEED_BUFF, s.stamina, s.power,
                          s.guts * FIRM_GUTS_BUFF, STAT_COUNT);
  case COND_GOOD:
    return integerAverage(s.speed * GOOD_SPEED_BUFF, s.stamina, s.power,
                          s.guts * GOOD_GUTS_BUFF, STAT_COUNT);
  case COND_SOFT:
    return integerAverage(s.speed, s.stamina * SOFT_STAMINA_BUFF,
                          s.power * SOFT_POWER_BUFF, s.guts, STAT_COUNT);
  case COND_WET:
    return integerAverage(s.speed, s.stamina * WET_STAMINA_BUFF,
                          s.power * WET_POWER_BUFF, s.guts, STAT_COUNT);
  default:
    return 0;
  }
}

int totalEffectiveness(int typeEff, int lengthEff, int conditionEff) {
  return typeEff + lengthEff + conditionEff;
}
