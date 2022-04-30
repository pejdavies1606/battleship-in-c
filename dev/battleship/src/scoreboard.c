/*
 * scoreboard.c
 *
 *  Created on: 26 Jun 2018
 *      Author: pejdavies1606
 */

#include "battleship/scoreboard.h"
#include "battleship/util.h"
#include <string.h>

bool Scoreboard_Init(Scoreboard_t *scoreboard, uint num_entities)
{
    bool result = false;
    if (scoreboard)
    {
        scoreboard->num_entities = num_entities;
        scoreboard->entities = malloc(
            num_entities * sizeof(ScoreboardEntity_t));
        if (scoreboard->entities)
        {
            memset(scoreboard->entities, 0,
                   num_entities * sizeof(ScoreboardEntity_t));
            result = true;
        }
    }
    return result;
}

void Scoreboard_Destroy(Scoreboard_t * const scoreboard)
{
    if (scoreboard)
    {
        Destroy((void **)&scoreboard->entities);
        scoreboard->num_entities = 0u;
    }
}