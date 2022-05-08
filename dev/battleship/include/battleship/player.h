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

typedef struct Player
{
   Grid_t grid;
   Ship_t ships[NUM_SHIPS];
   uint sunk_ships;
   bool sunk_all;
} Player_t;

bool Player_Init(
   Player_t * const player);

bool Player_PlaceShip(
   Player_t * const player,
   Ship_t * const ship,
   GridStatus_e * const status);

bool Player_PlaceShipsAuto(
   Player_t * const player);

bool Player_HitShip(
   Player_t * const player,
   ShipType_e const ship_type,
   bool * const sunk,
   bool * const sunk_all);

#endif /* BATTLESHIP_PLAYER_H_ */
