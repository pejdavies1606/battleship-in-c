/*
 * player.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_PLAYER_H_
#define BATTLESHIP_PLAYER_H_

#include "ship.h"
#include "commondefs.h"
#include "scoreboard.h"

#define GRID_SIZE 10

typedef enum
{
   STATE_BLANK,
   STATE_HIT,
   STATE_MISS
} Hit_State_t;

typedef struct
{
   Ship_Type_t ship_type;
   Hit_State_t hit_state;
} Grid_State_t;

typedef struct
{
   Grid_State_t *defense;
   Hit_State_t *offense;
   Ship_t *ships;
   Scoreboard_t scoreboard_ship_health;
} Player_t;

Player_t Player_Init();

#endif /* BATTLESHIP_PLAYER_H_ */
