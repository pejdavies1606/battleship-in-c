/*
 * rng.c
 *
 *  Created on: 27 Jun 2018
 *      Author: pejdavies1606
 */

#include "rng.h"
#include <time.h>

void Rng_Init(uint seed)
{
	if (seed == 0)
	{
		srand(time(NULL));
	}
	else
	{
		srand(seed);
	}
}
