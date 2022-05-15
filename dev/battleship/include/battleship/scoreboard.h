/*
 * scoreboard.h
 *
 *  Created on: 26 Jun 2018
 *      Author: pejdavies1606
 */

#ifndef SCOREBOARD_H_
#define SCOREBOARD_H_

#include "battleship/commondefs.h"

typedef struct ScoreboardEntity
{
   char const *name;
   uint score;
   uint total;
} ScoreboardEntity_t;

typedef struct Scoreboard
{
   char * title;
   uint num_entities;
   ScoreboardEntity_t *entities;
   uint width_score;
   uint width_total;
} Scoreboard_t;

bool Scoreboard_Init(Scoreboard_t *scoreboard, uint num_entities);
void Scoreboard_Destroy(Scoreboard_t * const scoreboard);

#endif /* SCOREBOARD_H_ */
