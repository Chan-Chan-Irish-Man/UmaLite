#include "uma.h"
#include "race.h"
#include "generateRace.h"

int turnCount = 1;

int main()
{
  srand(time(NULL));

  generatePlayerUma();

  while (1)
  {
    generateNPCUma(NPC_AMOUNT, turnCount);

    generateRace();

    turnCount++;
  }

	return 0;
}
