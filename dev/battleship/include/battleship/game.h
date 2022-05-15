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

#include "battleship/commondefs.h"
#include "battleship/comp_player.h"
#include "battleship/player.h"
#include "battleship/ship.h"
#include "battleship/scoreboard.h"

#define NUM_PLAYERS  2

#define NAME_WIDTH 16

#define SCOREBOARD_HITS_TITLE "Score"
#define SCOREBOARD_HITS_SCORE_WIDTH 1
#define SCOREBOARD_HITS_TOTAL_WIDTH 1

typedef struct Game
{
   Player_t players[NUM_PLAYERS];
   Comp_Player_t comps[NUM_PLAYERS];
   Scoreboard_t ship_health;
   Scoreboard_t hit_score;
} Game_t;

bool BattleShip_Game_Init(Game_t *const game);
void BattleShip_Game_Destroy(Game_t *const game);
bool BattleShip_Game_Start(Game_t *const game);

#endif /* BATTLESHIP_GAME_H_ */
