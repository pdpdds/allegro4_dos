#include "helper.h"
#include <stdlib.h>
#include <time.h>

void randomize(void) 
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < (rand() % RAND_MAX); i++) (rand() % RAND_MAX);
}

void srandom(unsigned int seed)
{
	srand(seed);
}