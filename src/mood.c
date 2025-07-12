#include "mood.h"
#include "stdlib.h"

const int moodWeights[MOOD_COUNT] = {
    2, // GREAT
    4, // GOOD
    9, // NORMAL
    4, // BAD
    2  // AWFUL
};

Mood getRandomMood() {
  int totalWeight = 0;
  for (int i = 0; i < MOOD_COUNT; i++) {
    totalWeight += moodWeights[i];
  }

  int r = rand() % totalWeight;
  for (int i = 0; i < MOOD_COUNT; i++) {
    if (r < moodWeights[i]) {
      return (Mood)i;
    }
    r -= moodWeights[i];
  }

  return NORMAL;
}

double moodMultiplier(void) {
  switch (getRandomMood()) {
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
