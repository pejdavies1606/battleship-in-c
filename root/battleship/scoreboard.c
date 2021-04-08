/*
 * scoreboard.c
 *
 *  Created on: 26 Jun 2018
 *      Author: pejdavies1606
 */

#include "scoreboard.h"
#include <string.h>

Status_t Scoreboard_Init(Scoreboard_t *scoreboard, uint num_entities)
{
    if (scoreboard)
    {
        scoreboard->num_entities = num_entities;
        scoreboard->entities = malloc(num_entities * sizeof(Scoreboard_Entity_t));
        if (!scoreboard->entities)
        {
            return STATUS_ERROR;
        }
        memset(scoreboard->entities, 0, num_entities * sizeof(Scoreboard_Entity_t));
    }
    return STATUS_OK;
}
