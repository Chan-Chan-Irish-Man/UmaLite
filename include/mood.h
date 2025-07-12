#define GREAT_MULTIPLIER 1.4
#define GOOD_MULTIPLIER 1.2
#define NORMAL_MULTIPLIER 1
#define BAD_MULTIPLIER .8
#define AWFUL_MULTIPLIER .6
#define MOOD_WGHT_COUNT 21

typedef enum {
  GREAT = 1 << 0,
  GOOD = 1 << 1,
  NORMAL = 1 << 2,
  BAD = 1 << 3,
  AWFUL = 1 << 4
} Moods;

double moodMultiplier(void);
