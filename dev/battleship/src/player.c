/*
 * player.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/player.h"

static Ship_t* _GetShip(Player_t *player, ShipType_e type);

Player_t * Player_Init(uint num_players)
{
   Player_t *players = malloc(num_players * sizeof(Player_t));
   if (players)
   {
      memset(players, 0, num_players * sizeof(Player_t));
      for (uint p = 0; p < num_players; p++)
      {
         Player_t *player = &players[p];
         if (Grid_Init(&player->grid))
         {
            player->ships = Ship_Init(NUM_SHIPS);
            if (player->ships)
            {
               for (uint i = 0; i < NUM_SHIPS; i++)
               {
                  player->ships[i].type = SHIP_TABLE[i].type;
               }
            }
            else
            {
               free(players);
               players = NULL;
            }
         }
         else
         {
            free(players);
            players = NULL;
         }
      }
   }
   return players;
}

void Player_Destroy(Player_t ** const players, uint num_players)
{
   if (players && *players && num_players > 0)
   {
      Player_t * pPlayers = *players;
      for (uint p = 0; p < num_players; p++)
      {
         Player_t * player = &pPlayers[p];
         Ship_Destroy(&player->ships);
         Grid_Destroy(&player->grid);
      }
      free(*players);
      *players = NULL;
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
