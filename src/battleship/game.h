/*
 * battleship.h
 *
 *  Created on: 25 Jun 2018
 *      Author: pejdavies1606, Univeristy of Surrey, 2013
 */

#ifndef BATTLESHIP_GAME_H_
#define BATTLESHIP_GAME_H_

#include <stdbool.h>
#include <stdio.h>

#include "ship.h"
#include "commondefs.h"
#include "rng.h"
#include "scoreboard.h"

#define NUM_PLAYERS  2
#define NUM_ADJACENT 4
#define NUM_LINE     9

#define NAME_WIDTH 16

#define SCOREBOARD_HITS_TITLE "Score"
#define SCOREBOARD_HITS_SCORE_WIDTH 1
#define SCOREBOARD_HITS_TOTAL_WIDTH 1

#include "convert.h"
#include "player.h"
#include "comp_player.h"

#define STATE_BLANK " "
#define STATE_HIT   "*"
#define STATE_MISS  "."

#define STATE_ZERO  "0"

#define GRID_SIDE_V "|"
#define GRID_SIDE_H "="
#define GRID_CORNER "+"

typedef struct
{
   Player_t players[NUM_PLAYERS];
   Comp_Player_t comps[NUM_PLAYERS];
   Scoreboard_t scoreboard_hit_score;
} Game_t;

void BattleShip_Game_Init();
void BattleShip_Game_Start();

#endif /* BATTLESHIP_GAME_H_ */
