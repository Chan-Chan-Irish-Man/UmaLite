#include "generate.h"
#include "generateRace.h"
#include "inheritance.h"
#include "race.h"
#include "stdlib.h"
#include "time.h"

int restart = 0;
int turn = 1;

static void initGame(void) {
  srand((unsigned int)time(NULL));
  generatePlayerUma();
}

int main(void) {
  initGame();

  int totalRaces = randNumInRange(MIN_RACE, MAX_RACE);

  while (turn <= totalRaces) {
    if (restart) {
      umaInheritance();
      restart = 0;
      turn = 1;
    }

    int randNPCCount = randNumInRange(MIN_NPC, MAX_NPC);
    Uma *npc = generateNPCUma(randNPCCount, turn);

    int placementCutoff = randNPCCount + PLACEMENT_GRACE_OFFSET - turn;
    generateRace(npc, turn, randNPCCount, totalRaces, placementCutoff);

    // generateRace(generateNPCUma...) is certainly an option,
    // but I would be unable to free the npc uma struct
    free(npc);
    turn++;
  }

  return 0;
}
