/*
 * player.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_PLAYER_H_
#define BATTLESHIP_PLAYER_H_

#include "battleship/ship.h"
#include "battleship/commondefs.h"
#include "battleship/scoreboard.h"
#include "battleship/grid.h"

#define GRID_SIZE 10
#define GRID_SIZE_LEN 2
#define COORD_LEN (1 + GRID_SIZE_LEN) // J10

typedef struct Player
{
   Grid_t grid;
   Ship_t *ships;
} Player_t;

Player_t * Player_Init(uint num_players);
GridStatus_e Player_PlaceShip(Player_t *player, Ship_t *ship);
bool Player_PlaceShipsAuto(Player_t * const player);
void Player_Destroy(Player_t ** const players, uint num_players);

#endif /* BATTLESHIP_PLAYER_H_ */
