/*
 * scoreboard.c
 *
 *  Created on: 26 Jun 2018
 *      Author: pejdavies1606
 */

#include "scoreboard.h"

Scoreboard_Entity_t Scoreboard_Entity_Init(String_t name, uint score, uint total)
{
   Scoreboard_Entity_t entity =
   {
         .name =  name,
         .score = score,
         .total = total
   };

   return entity;
}

void Scoreboard_Init(Scoreboard_t *board,
                     String_t title,
                     uint num_entities,
                     uint width_score,
                     uint width_total)
{
   board->title = title;
   board->num_entities = num_entities;
   board->width_score = width_score;
   board->width_total = width_total;
}
