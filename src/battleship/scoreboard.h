/*
 * scoreboard.h
 *
 *  Created on: 26 Jun 2018
 *      Author: pejdavies1606
 */

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include "commondefs.h"

typedef struct
{
   String_t name;
   uint score;
   uint total;
} Scoreboard_Entity_t;

typedef struct
{
   String_t title;
   uint num_entities;
   Scoreboard_Entity_t *entities;
   uint width_score;
   uint width_total;
} Scoreboard_t;

Status_t Scoreboard_Init(Scoreboard_t *scoreboard, uint num_entities);

#endif /* SCOREBOARD_H_ */
