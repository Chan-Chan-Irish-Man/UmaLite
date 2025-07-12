#include "generate.h"
#include "generateRace.h"
#include "race.h"
#include "stdlib.h"
#include "time.h"

int turn = 1;

static void initGame(void) {
  srand((unsigned int)time(NULL));
  generatePlayerUma();
}

int main(void) {
  initGame();

  while (1) {
    generateNPCUma(NPC_AMOUNT, turn);
    generateRace(turn);
    turn++;
  }

  return 0;
}
