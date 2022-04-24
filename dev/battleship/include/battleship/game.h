/*
 * battleship.h
 *
 *  Created on: 25 Jun 2018
 *      Author: pejdavies1606, University of Surrey, 2013
 */

#ifndef BATTLESHIP_GAME_H_
#define BATTLESHIP_GAME_H_

#include <stdbool.h>
#include <stdio.h>

#include "battleship/ship.h"
#include "battleship/commondefs.h"
#include "battleship/scoreboard.h"

#define NUM_PLAYERS  2
#define NUM_ADJACENT 4
#define NUM_LINE     9

#define NAME_WIDTH 16

#define SCOREBOARD_HITS_TITLE "Score"
#define SCOREBOARD_HITS_SCORE_WIDTH 1
#define SCOREBOARD_HITS_TOTAL_WIDTH 1

#include "battleship/player.h"
#include "battleship/comp_player.h"

typedef struct Game
{
   Player_t *players;
   Comp_Player_t *comps;
   Scoreboard_t ship_health;
   Scoreboard_t hit_score;
} Game_t;

Game_t * BattleShip_Game_Init();
void BattleShip_Game_Destroy(Game_t ** const game);
bool BattleShip_Game_Start();

#endif /* BATTLESHIP_GAME_H_ */
