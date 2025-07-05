#include "uma.h"

char* umaForename[12] = { "Top ", "Admire ", "Mejiro ", 
	"Silence ", "Nice ", "Great ", 
	"King ", "Queen ", "Special ",
	"Daiwa ", "Rice ", "Digital " };

char* umaSurname[12] = { "Scarlet", "Nature", "Vodka", 
	"Ship", "Shower", "Cap", 
	"Teio", "Opera", "Flash",
	"Urara", "Wonder", "Tachyon" };

char* generatedNames[NPC_AMOUNT];

int generatedCount = 0;

int retries = 0;
const int maxRetries = 1000;

int isDuplicate(char* npcName)
{
  for (int i = 0; i < generatedCount; ++i)
  {
    if (strcmp(generatedNames[i], npcName) == 0)
    {
      return 1;
    }
  }

  return 0;
}

/*
void saveUma(const char *filename, const Uma *uma)
{
	FILE *file = fopen(filename, "w");
	if (file == NULL) 
	{
		perror("Unable to open file.");
		exit(1);
	}

	fprintf(file, "Name: %s\n", uma->name);
	fprintf(file, "Speed: %d\n", uma->speed);
	fprintf(file, "Stamina: %d\n", uma->stamina);
	fprintf(file, "Power: %d\n", uma->power);
	fprintf(file, "Guts: %d\n", uma->guts);
	fprintf(file, "Wit: %d\n", uma->wit);
	fprintf(file, "Average Stats: %d\n", uma->average);

	fclose(file);
}
*/

const GradeThreshold gradeTable[] = {
    {  51,  "G"   },
    { 100,  "G+"  },
    { 150,  "F"   },
    { 200,  "F+"  },
    { 250,  "E"   },
    { 300,  "E+"  },
    { 350,  "D"   },
    { 400,  "D+"  },
    { 450,  "C"   },
    { 600,  "C+"  },
    { 700,  "B"   },
    { 800,  "B+"  },
    { 900,  "A"   },
    {1000,  "A+"  },
    {1050,  "S"   },
    {1100,  "S+"  },
    {1150,  "SS"  },
    {INT_MAX, "SS+"} // catch-all
};

char* gradeConvert(int stat)
{
  for (int i = 0; i < sizeof(gradeTable)/sizeof(gradeTable[0]); ++i)
  {
    if (stat < gradeTable[i].threshold)
    {
      return (char*)gradeTable[i].grade;
    }
  }

  return "??";
}

char* enterName()
{
	char* name = malloc(50);

	if (!name) 
	{
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

	while (1)
	{
		char nameConfirm[10];

		printf("Please enter a name for your Umamusume: ");
		fgets(name, 50, stdin);
		// Remove trailing newline from name
		name[strcspn(name, "\n")] = '\0';

		printf("Is this name okay? (yes/y or no/n): ");
		fgets(nameConfirm, sizeof(nameConfirm), stdin);
		nameConfirm[strcspn(nameConfirm, "\n")] = '\0';
		
		for (int i = 0; nameConfirm[i]; i++)
		{
			nameConfirm[i] = tolower(nameConfirm[i]);
		}
		
		if (strcmp(nameConfirm, "yes") == 0 || strcmp(nameConfirm, "y") == 0)
		{
			break;
		}
	}

	return name;
}

int statGenerate()
{
	// usually does not go above F+ at start of game (<200) or below G+ (50>)
	
	return (rand() % (STAT_MAX + 1 - STAT_MIN)) + STAT_MIN;
}

int averageStat(int rawSpeed, int rawStamina, int rawPower, int rawGuts, int rawWit)
{
	return (rawSpeed + rawStamina + rawPower + rawGuts + rawWit) / 5;
}

double witBuff(int rawWit)
{
	return rawWit / (WIT_SCALING_FACTOR + rawWit);
}

WitBuffResult applyWitBuff(int rawStat, int rawWit)
{
  WitBuffResult result;

  result.witBonus = rawStat * witBuff(rawWit);
  result.finalValue = rawStat + result.witBonus;

  return result;
}

Uma PlayerUma;

void generatePlayerUma()
{
	int rawSpeed, rawStamina, rawPower, rawGuts, rawWit, statAverage;

  WitBuffResult speed, stamina, power, guts;

	while (1)
	{
		char statConfirm[10];

		rawWit = statGenerate();
		
		rawSpeed = statGenerate();
		speed = applyWitBuff(rawSpeed, rawWit);
		
		rawStamina = statGenerate();
		stamina = applyWitBuff(rawStamina, rawWit);
		
		rawPower = statGenerate();
		power = applyWitBuff(rawPower, rawWit);

		rawGuts = statGenerate();
		guts = applyWitBuff(rawGuts, rawWit);
		
    statAverage = averageStat(speed.finalValue, stamina.finalValue,
                              power.finalValue, guts.finalValue, rawWit);
		
		printf("Speed: %d (%s)\n(Base Speed %d + %d Wit Buff)\n",
				speed.finalValue, gradeConvert(speed.finalValue), rawSpeed, speed.witBonus);

		printf("Stamina: %d (%s)\n(Base Stamina %d + %d Wit Buff)\n",
        stamina.finalValue, gradeConvert(stamina.finalValue), rawStamina, stamina.witBonus);

		printf("Power: %d (%s)\n(Base Power %d + %d Wit Buff)\n",
        power.finalValue, gradeConvert(stamina.finalValue), rawPower, power.witBonus);

		printf("Guts: %d (%s)\n(Base Guts %d + %d Wit Buff)\n",
        guts.finalValue, gradeConvert(guts.finalValue), rawGuts, guts.witBonus);

		printf("Wit: %d (%s)\n", rawWit, gradeConvert(rawWit));

		printf("Average Stat: %d\n", statAverage);
		printf("Average Grade: %s\n", gradeConvert(statAverage));

		printf("Are these stats okay? (yes/y or no/n): ");

		fgets(statConfirm, sizeof(statConfirm), stdin);
		statConfirm[strcspn(statConfirm, "\n")] = '\0';

		for (int i = 0; statConfirm[i]; i++)
		{
			statConfirm[i] = tolower(statConfirm[i]);
		}

		if (strcmp(statConfirm, "yes") == 0 || strcmp(statConfirm, "y") == 0)
		{
			break;
		}
	}

	char* umaName = enterName(); 

	strcpy(PlayerUma.name, umaName);
	PlayerUma.speed = speed.finalValue;
	PlayerUma.stamina = stamina.finalValue;
	PlayerUma.power = power.finalValue;
	PlayerUma.guts = guts.finalValue;
	PlayerUma.wit = rawWit;
	PlayerUma.average = statAverage;

	free(umaName);

	// saveUma("umaStats.txt", &PlayerUma);
}

double raceFactor(int raceNo)
{
	return 1.0 + (raceNo - 1) * 0.3;
}

const char* getRandomForename()
{
  return umaForename[(rand() % (NAME_MAX + 1 - NAME_MIN)) + NAME_MIN];
}

const char* getRandomSurname()
{
  return umaSurname[(rand() % (NAME_MAX + 1 - NAME_MIN)) + NAME_MIN];
}

char* generateName()
{
	char* fullName = malloc(50);

  snprintf(fullName, 50, "%s%s", getRandomForename(), getRandomSurname());
  return fullName;
}

int npcStatBuff(int baseStat, int wit, int raceFactor)
{
	return (int)baseStat * (1.0 + witBuff(wit)) * raceFactor;
}

void resetGeneratedNames()
{
    for (int i = 0; i < generatedCount; ++i)
    {
        free(generatedNames[i]);
        generatedNames[i] = NULL;
    }
    generatedCount = 0;
}

Uma NPCUma[NPC_AMOUNT];

void generateNPCUma(int amount, int raceNo)
{
	for (int i = 0; i < amount; i++)
	{
    char npcName[50];

    while (retries++ < maxRetries)
    {
        strcpy(npcName, generateName());

        if (!isDuplicate(npcName))
        {
            generatedNames[generatedCount] = malloc(strlen(npcName) + 1);
            if (!generatedNames[generatedCount]) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }

            strcpy(generatedNames[generatedCount], npcName);
            generatedCount++;
            break;
        }
    }

    if (retries >= maxRetries) {
        fprintf(stderr, "Error: Could not generate a unique NPC name after %d attempts.\n", maxRetries);
        exit(1);
    }

		strcpy(NPCUma[i].name, npcName);
		
		NPCUma[i].wit = statGenerate() * raceFactor(raceNo);

		NPCUma[i].speed = npcStatBuff(statGenerate(), NPCUma[i].wit, raceFactor(raceNo));
		
		NPCUma[i].stamina = npcStatBuff(statGenerate(), NPCUma[i].wit, raceFactor(raceNo));
		
		NPCUma[i].power = npcStatBuff(statGenerate(), NPCUma[i].wit, raceFactor(raceNo));
		
		NPCUma[i].guts = npcStatBuff(statGenerate(), NPCUma[i].wit, raceFactor(raceNo));
		
		NPCUma[i].average = averageStat(NPCUma[i].speed, 
				NPCUma[i].power, NPCUma[i].stamina, 
				NPCUma[i].guts, NPCUma[i].wit);
  }

  resetGeneratedNames();
}
