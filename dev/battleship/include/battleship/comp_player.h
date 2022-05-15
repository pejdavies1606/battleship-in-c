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

#define NUM_ADJACENT 4
#define NUM_LINE     9

typedef enum SearchState
{
   SEARCH_RANDOM,
   SEARCH_CIRCLE,
   SEARCH_LINE,
} SearchState_e;

typedef struct CircleSearch
{
   Heading_e heading;
   bool clockwise;
   uint tries;
} CircleSearch_t;

typedef struct Comp_Player
{
   bool hit;
   SearchState_e search;
   CircleSearch_t circle;
   uint pass;
   uint scan_stage;
   uint start;
   int clock;
   uint pos;
   int line_pos;
   Coord_t centre;
   Coord_t target;
   Coord_t adjacent[NUM_ADJACENT];
   Coord_t line[NUM_LINE];
   bool first_time;
} Comp_Player_t;

#endif /* BATTLESHIP_COMP_PLAYER_H_ */
