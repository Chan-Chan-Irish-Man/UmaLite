#define GREAT_WEIGHT 2
#define GOOD_WEIGHT 4
#define NORMAL_WEIGHT 9
#define BAD_WEIGHT 4
#define AWFUL_WEIGHT 2

#define GREAT_MULTIPLIER 1.4
#define GOOD_MULTIPLIER 1.2
#define NORMAL_MULTIPLIER 1
#define BAD_MULTIPLIER .8
#define AWFUL_MULTIPLIER .6

typedef enum { GREAT, GOOD, NORMAL, BAD, AWFUL, MOOD_COUNT } Mood;

double moodMultiplier(void);
