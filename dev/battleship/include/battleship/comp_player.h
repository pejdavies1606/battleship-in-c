/*
 * comp_player.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_COMP_PLAYER_H_
#define BATTLESHIP_COMP_PLAYER_H_

#include "battleship/commondefs.h"
#include "battleship/coord.h"

typedef enum SearchState
{
   SEARCH_RANDOM,
   SEARCH_CIRCLE,
   SEARCH_LINE,
} SearchState_e;

typedef struct CircleSearch
{
   Heading_e heading;
   bool direction;
   uint tries;
   Coord_t centre;
   uint pos;
} Search_t;

typedef struct Comp_Player
{
   bool hit;
   bool sunk;
   SearchState_e state;
   Search_t search;
} Comp_Player_t;

#endif /* BATTLESHIP_COMP_PLAYER_H_ */
