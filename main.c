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

static int gameShouldContinue() {
    return 1; // TODO: SO USER CAN EXIT PROGRAM ANY TIME
}

int main(void) {
    initGame();

    while (gameShouldContinue()) {
        generateNPCUma(NPC_AMOUNT, turnCount);
        generateRace();
        turnCount++;
    }

    return 0;
}
