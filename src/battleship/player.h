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
#include "grid.h"

#define GRID_SIZE 10

typedef struct
{
   Grid_t grid;
   Ship_t *ships;
} Player_t;

Player_t * Player_Init(uint num_players);
Grid_Status_t Player_Place_Ship(Player_t *player, Ship_t *ship);
Status_t Player_Place_Ships_Auto(Player_t *player);

#endif /* BATTLESHIP_PLAYER_H_ */
