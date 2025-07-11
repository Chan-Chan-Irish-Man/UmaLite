#include "generate.h"
#include "generateRace.h"
#include "race.h"
#include "stdlib.h"
#include "time.h"

int turnCount = 1;

static void initGame(void) {
  srand((unsigned int)time(NULL));
  generatePlayerUma();
}

int main(void) {
  initGame();

  while (1) {
    generateNPCUma(NPC_AMOUNT, turnCount);
    generateRace();
    turnCount++;
  }

  return 0;
}
