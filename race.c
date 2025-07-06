#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "race.h"

// =================== RACECOURSE DEFINITIONS ===================

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

// =================== RANDOM PICKERS ===================

#define MAX_TRACK_OPTIONS 4

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

// =================== TRACK LABEL HELPERS ===================

const char* typeName(TrackType type) {
    switch (type) {
        case TRACK_DIRT: return "Dirt";
        case TRACK_TURF: return "Turf";
        default: return "Unknown";
    }
}

const char* lengthName(TrackLength length) {
    switch (length) {
        case LENGTH_SPRINT: return "Sprint";
        case LENGTH_MILE: return "Mile";
        case LENGTH_MEDIUM: return "Medium";
        case LENGTH_LONG: return "Long";
        default: return "Unknown";
    }
}

const char* conditionName(TrackCondition cond) {
    switch (cond) {
        case COND_FIRM: return "Firm";
        case COND_GOOD: return "Good";
        case COND_SOFT: return "Soft";
        case COND_WET: return "Wet";
        default: return "Unknown";
    }
}

// =================== RACE CREATION ===================

Race createRace(const Racecourse* course) {
    return (Race){
        .course = course,
        .chosenTrackType = pickRandomType(course->supportedTrackTypes),
        .chosenTrackLength = pickRandomLength(course->supportedTrackLengths),
        .chosenConditions = pickRandomCondition(course->supportedConditions)
    };
}

void printRace(int index, Race r) {
    printf("[%d] %s: %s %s Race (%s)\n", index + 1,
           r.course->courseName,
           lengthName(r.chosenTrackLength),
           typeName(r.chosenTrackType),
           conditionName(r.chosenConditions));
}

void initAvailableRaces(Race availableRaces[], int count) {
    for (int i = 0; i < count; ++i) {
        availableRaces[i] = createRace(&TRACK_LIST[i]);
        printRace(i, availableRaces[i]);
    }
}

void printCurrentRace(Race r) {
    printf("You chose [%s: %s %s Race (%s)]\n",
           r.course->courseName,
           lengthName(r.chosenTrackLength),
           typeName(r.chosenTrackType),
           conditionName(r.chosenConditions));
}

// =================== VALIDATION ===================

int getValidatedInt(const char* prompt, int min, int max) {
    int value;
    char buf[1024];
    char* endptr;
    int valid;

    do {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) return min;

        errno = 0;
        value = strtol(buf, &endptr, 10);
        valid = errno != ERANGE && endptr != buf &&
                (*endptr == '\n' || *endptr == '\0') &&
                value >= min && value <= max;

        if (!valid)
            printf("Invalid input. Please enter a number between %d and %d.\n", min, max);
    } while (!valid);

    return value;
}

// =================== EFFECTIVENESS LOGIC ===================

int integerAverage(int a, int b, int c, int d, int count) {
    return (a + b + c + d) / count;
}

int typeEffectiveness(Stats s, TrackType t) {
    switch (t) {
        case TRACK_DIRT:
            return integerAverage(s.speed, s.stamina,
                                  s.power * DIRT_POWER_BUFF,
                                  s.guts * DIRT_GUTS_BUFF,
                                  STAT_COUNT);
        case TRACK_TURF:
            return integerAverage(s.speed * TURF_SPEED_BUFF,
                                  s.stamina * TURF_STAMINA_BUFF,
                                  s.power, s.guts,
                                  STAT_COUNT);
        default: return 0;
    }
}

int lengthEffectiveness(Stats s, TrackLength l) {
    switch (l) {
        case LENGTH_SPRINT:
            return integerAverage(s.speed * SPRINT_SPEED_BUFF, s.stamina,
                                  s.power * SPRINT_POWER_BUFF, s.guts, STAT_COUNT);
        case LENGTH_MILE:
            return integerAverage(s.speed * MILE_SPEED_BUFF,
                                  s.stamina * MILE_STAMINA_BUFF,
                                  s.power * MILE_POWER_BUFF,
                                  s.guts * MILE_GUTS_BUFF,
                                  STAT_COUNT);
        case LENGTH_MEDIUM:
            return integerAverage(s.speed * MEDIUM_SPEED_BUFF,
                                  s.stamina * MEDIUM_STAMINA_BUFF,
                                  s.power * MEDIUM_POWER_BUFF,
                                  s.guts * MEDIUM_GUTS_BUFF,
                                  STAT_COUNT);
        case LENGTH_LONG:
            return integerAverage(s.speed,
                                  s.stamina * LONG_STAMINA_BUFF,
                                  s.power,
                                  s.guts * LONG_GUTS_BUFF,
                                  STAT_COUNT);
        default: return 0;
    }
}

int conditionEffectiveness(Stats s, TrackCondition c) {
    switch (c) {
        case COND_FIRM:
            return integerAverage(s.speed * FIRM_SPEED_BUFF,
                                  s.stamina,
                                  s.power,
                                  s.guts * FIRM_GUTS_BUFF,
                                  STAT_COUNT);
        case COND_GOOD:
            return integerAverage(s.speed * GOOD_SPEED_BUFF,
                                  s.stamina,
                                  s.power,
                                  s.guts * GOOD_GUTS_BUFF,
                                  STAT_COUNT);
        case COND_SOFT:
            return integerAverage(s.speed,
                                  s.stamina * SOFT_STAMINA_BUFF,
                                  s.power * SOFT_POWER_BUFF,
                                  s.guts,
                                  STAT_COUNT);
        case COND_WET:
            return integerAverage(s.speed,
                                  s.stamina * WET_STAMINA_BUFF,
                                  s.power * WET_POWER_BUFF,
                                  s.guts,
                                  STAT_COUNT);
        default: return 0;
    }
}

int totalEffectiveness(int typeEff, int lengthEff, int conditionEff) {
    return typeEff + lengthEff + conditionEff;
}
