#include "time.h"
#include "stdlib.h"
#include "generate.h"
#include "race.h"
#include "generateRace.h"

int turnCount = 1;

static void initGame() {
  srand((unsigned int)time(NULL));
  generatePlayerUma();
}

int main(void) {
  initGame();

  while (0) {
    generateNPCUma(NPC_AMOUNT, turnCount);
    generateRace();
    turnCount++;
  }

  return 0;
}
