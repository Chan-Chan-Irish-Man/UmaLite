#include "mood.h"
#include "stdlib.h"

int moodWeight[MOOD_WGHT_COUNT] = {
    GREAT,  GREAT,  GOOD,   GOOD,   GOOD,   GOOD,   NORMAL,
    NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL,
    NORMAL, BAD,    BAD,    BAD,    BAD,    AWFUL,  AWFUL};

double moodMultiplier(void) {
  int x = rand() % MOOD_WGHT_COUNT;

  switch (moodWeight[x]) {
  case GREAT:
    return GREAT_MULTIPLIER;
  case GOOD:
    return GOOD_MULTIPLIER;
  case NORMAL:
    return NORMAL_MULTIPLIER;
  case BAD:
    return BAD_MULTIPLIER;
  case AWFUL:
    return AWFUL_MULTIPLIER;
  default:
    return 0;
  }
}
