#ifndef STATUP_H
#define STATUP_H

#include "generate.h" // Needed for access to PlayerUma

// =================== CONFIG ===================

// Total points available after each race (divided by placement)
#define SKILL_UP_POOL 250

// Skills available for training
typedef enum {
  SKILL_SPEED = 1,
  SKILL_STAMINA,
  SKILL_POWER,
  SKILL_GUTS,
  SKILL_WIT
} SkillType;

// =================== FUNCTIONS ===================

// Triggers skill selection and allocation after a race
void statUp(int playerPlacement);

#endif // SKILLUP_H
