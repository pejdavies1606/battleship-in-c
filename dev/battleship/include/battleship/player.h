/*
 * player.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_PLAYER_H_
#define BATTLESHIP_PLAYER_H_

#include "battleship/commondefs.h"
#include "battleship/comp_player.h"
#include "battleship/grid.h"
#include "battleship/scoreboard.h"
#include "battleship/ship.h"

typedef struct Player
{
   Grid_t grid;
   Ship_t ships[NUM_SHIPS];
   uint sunk_ships;
   bool sunk_all;
   Comp_Player_t comp;
} Player_t;

bool Player_Init(
   Player_t * const player);

bool Player_PlaceShip(
   Player_t * const player,
   Ship_t * const ship,
   GridStatus_e * const status);

bool Player_PlaceShipsAuto(
   Player_t * const player);

bool Player_PlaceHit(
   Player_t *const player,
   Coord_t const *const target,
   ShipType_e *const hit_ship,
   bool *const sunk,
   bool *const sunk_all);

bool BattleShipAI_GetCoord(
   Player_t *const player,
   GridState_e const *const states,
   Coord_t *const target);

#endif /* BATTLESHIP_PLAYER_H_ */
