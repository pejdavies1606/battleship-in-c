/*
 * comp_player.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_COMP_PLAYER_H_
#define BATTLESHIP_COMP_PLAYER_H_

#include "commondefs.h"

typedef struct
{
	uint stage;
	uint pass;
	uint scan_stage;
	uint start;
	int clock;
	uint pos;
	int line_pos;
	Coord_t centre;
	Coord_t try;
	Coord_t adjacent[NUM_ADJACENT];
	Coord_t line[NUM_LINE];
	_Bool first_time;
} Comp_Player_t;

#endif /* BATTLESHIP_COMP_PLAYER_H_ */
