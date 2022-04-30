/*
 * player.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/player.h"

static Ship_t* _GetShip(Player_t *player, ShipType_e type);

bool Player_Init(Player_t * const player)
{
   bool result = false;
   if (player)
   {
      if (Grid_Init(&player->grid))
      {
         player->ships = Ship_Init(NUM_SHIPS);
         if (player->ships)
         {
            for (uint i = 0; i < NUM_SHIPS; i++)
            {
               player->ships[i].type = SHIP_TABLE[i].type;
            }
            result = true;
         }
      }
   }
   return result;
}

void Player_Destroy(Player_t * const player)
{
   if (player)
   {
      Ship_Destroy(&player->ships);
      Grid_Destroy(&player->grid);
   }
}

GridStatus_e Player_PlaceShip(Player_t *player, Ship_t *ship)
{
  GridStatus_e status = GRID_STATUS_NULL;
   if (player && ship)
   {
      Ship_t *player_ship = _GetShip(player, ship->type);
      if (player_ship)
      {
         *player_ship = *ship;
         status = Grid_PlaceShip(&player->grid, ship);
      }
   }
   return status;
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
            GridStatus_e status = GRID_STATUS_UNKNOWN;
            do
            {
               Ship_t ship = {0};
               ship.type = SHIP_TABLE[i - 1].type;
               ship.location = Coord_InitRandom(
                   0, (int)player->grid.rows,
                   0, (int)player->grid.cols);
               ship.heading = Heading_InitRandom();
               status = Player_PlaceShip(player, &ship);
            } while (
                GRID_STATUS_OK != status &&
                GRID_STATUS_NULL != status);
            if (GRID_STATUS_NULL == status)
            {
               result = false;
               break;
            }
         }
         result = true;
      }
   }
   return result;
}

Ship_t * _GetShip(Player_t *player, ShipType_e type)
{
   if (player)
   {
      for (uint i = 0; i < NUM_SHIPS; i++)
      {
         if (type == player->ships[i].type)
         {
            return &player->ships[i];
         }
      }
   }
   return NULL;
}
