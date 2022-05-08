/*
 * player.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/player.h"

static Ship_t* _GetShip(
   Player_t * const player,
   ShipType_e const type);

bool Player_Init(Player_t * const player)
{
   bool result = false;
   if (player)
   {
      if (Grid_Init(&player->grid))
      {
         memset(player->ships, 0, NUM_SHIPS * sizeof(Ship_t));
         for (uint i = 0; i < NUM_SHIPS; i++)
         {
            player->ships[i].type = SHIP_TABLE[i].type;
         }
         result = true;
      }
   }
   return result;
}

bool Player_PlaceShip(
   Player_t * const player,
   Ship_t * const ship,
   GridStatus_e * const status)
{
   bool result = false;
   if (player && ship)
   {
      Ship_t * const player_ship = _GetShip(player, ship->type);
      if (player_ship)
      {
         result = Grid_PlaceShip(&player->grid, player_ship, ship, status);
      }
   }
   return result;
}

bool Player_PlaceShipsAuto(Player_t * const player)
{
   bool result = false;
   if (player)
   {
      if (Grid_Clear(&player->grid))
      {
         for (uint i = NUM_SHIPS; i > 0; i--)
         {
            ShipType_e type = SHIP_TABLE[i - 1].type;
            Ship_t ship = {0};
            ship.type = type;
            Ship_t * player_ship = _GetShip(player, ship.type);
            if (player_ship)
            {
               GridStatus_e status = GRID_STATUS_UNKNOWN;
               do
               {
                  ship.location = Coord_InitRandom(
                      0, (int)player->grid.rows,
                      0, (int)player->grid.cols);
                  ship.heading = Heading_InitRandom();
                  result = Grid_PlaceShip(
                      &player->grid,
                      player_ship,
                      &ship,
                      &status);
               } while (result && GRID_STATUS_OK != status);
               if (!result)
                  break;
            }
         }
      }
   }
   return result;
}

bool Player_HitShip(
   Player_t * const player,
   ShipType_e const ship_type,
   bool * const sunk,
   bool * const sunk_all)
{
   bool result = false;
   if (player && sunk && sunk_all)
   {
      Ship_t * const ship = _GetShip(player, ship_type);
      if (ship)
      {
         result = Ship_Hit(ship, sunk);
         if (result && *sunk)
         {
            result = Grid_SinkShip(
                &player->grid,
                player->grid.states,
                ship);
            if (result)
            {
               player->sunk_ships++;
               player->sunk_all = (1 == player->sunk_ships);
               *sunk_all = player->sunk_all;
            }
         }
      }
   }
   return result;
}

Ship_t * _GetShip(
   Player_t * const player,
   ShipType_e const type)
{
   Ship_t * ship = NULL;
   if (player)
   {
      for (uint i = 0; i < NUM_SHIPS; i++)
      {
         if (type == player->ships[i].type)
         {
            ship = &player->ships[i];
            break;
         }
      }
   }
   return ship;
}
