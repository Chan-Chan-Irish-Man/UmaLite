#ifndef RACE_H
#define RACE_H

#define STAT_COUNT 4 // amount of stats affected by the track (wit not included)

#define DIRT_POWER_BUFF 1.5
#define DIRT_GUTS_BUFF 1.5

#define TURF_SPEED_BUFF 1.5
#define TURF_STAMINA_BUFF 1.5

#define SPRINT_SPEED_BUFF 1.4
#define SPRINT_POWER_BUFF 1.4

#define MILE_SPEED_BUFF 1.3
#define MILE_POWER_BUFF 1.3
#define MILE_STAMINA_BUFF 1.1
#define MILE_GUTS_BUFF 1.1

#define MEDIUM_SPEED_BUFF 1.1
#define MEDIUM_POWER_BUFF 1.1
#define MEDIUM_STAMINA_BUFF 1.3
#define MEDIUM_GUTS_BUFF 1.3

#define LONG_STAMINA_BUFF 1.4
#define LONG_GUTS_BUFF 1.4

#define FIRM_SPEED_BUFF 1.4
#define FIRM_GUTS_BUFF 1.4

#define GOOD_SPEED_BUFF 1.2
#define GOOD_GUTS_BUFF 1.2

#define SOFT_STAMINA_BUFF 1.2
#define SOFT_POWER_BUFF 1.2

#define WET_STAMINA_BUFF 1.4
#define WET_POWER_BUFF 1.4

typedef enum TrackType
{
  TRACK_DIRT = 1 << 0,
  TRACK_TURF = 1 << 1
} TrackType;

typedef enum TrackLength
{
  LENGTH_SPRINT = 1 << 0,
  LENGTH_MILE = 1 << 1,
  LENGTH_MEDIUM = 1 << 2,
  LENGTH_LONG = 1 << 3

} TrackLength;

typedef enum TrackCondition
{
  COND_FIRM = 1 << 0,
  COND_GOOD = 1 << 1,
  COND_SOFT = 1 << 2,
  COND_WET = 1 << 3
} TrackCondition;

typedef struct Racecourse
{
  const char* courseName;
  int supportedTrackTypes;
  int supportedTrackLengths;
  int supportedConditions;
} Racecourse;

typedef struct Race
{
  const Racecourse* course;

  int chosenTrackType;
  int chosenTrackLength;
  int chosenConditions;
} Race;

typedef struct Stats
{
  int speed;
  int stamina;
  int power;
  int guts;
  int statCheckCount;
} Stats;

extern Racecourse TRACK_LIST[];
extern const int NUM_TRACKS;

TrackCondition pickRandomCondition(int supportedConditions);
const char* conditionName(TrackCondition cond);

TrackType pickRandomType(int supportedTrackTypes);
const char* typeName(TrackType type);

TrackLength pickRandomLength(int supportedTrackLengths);
const char* lengthName(TrackLength length);

Race createRace(const Racecourse* course);

void printRace(int index, Race r);

void initAvailableRaces(Race availableRaces[], int count);

int getValidatedInt(const char* prompt, int min, int max);

void printCurrentRace(Race r);

int typeEffectiveness(Stats stats, TrackType type);
int lengthEffectiveness(Stats stats, TrackLength length);
int conditionEffectiveness(Stats stats, TrackCondition condition);

int totalEffectiveness(int typeScore, int lengthScore, int conditionScore);

#endif
