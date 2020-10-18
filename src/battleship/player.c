/*
 * player.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "player.h"

static Ship_t* Player_Get_Ship(Player_t *player, Ship_Type_t type);

Player_t * Player_Init(uint num_players)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif

   Player_t *players = malloc(num_players * sizeof(Player_t));
   if (!players)
   {
      return NULL;
   }
   memset(players, 0, num_players * sizeof(Player_t));

   for (uint p = 0; p < num_players; p++)
   {
      Player_t *player = &players[p];
      player->ships = Ship_Init(NUM_SHIPS);
      if (!player->ships)
      {
         return NULL;
      }

      for (uint i = 0; i < NUM_SHIPS; i++)
      {
         player->ships[i].type = shipTable[i].type;
      }

      if (STATUS_OK != Grid_Init(&player->grid, GRID_SIZE, GRID_SIZE))
      {
         return NULL;
      }
   }

   return players;
}

Grid_Status_t Player_Place_Ship(Player_t *player, Ship_t *ship)
{
  Grid_Status_t status = GRID_STATUS_UNKNOWN;
   if (player && ship)
   {
      Ship_t *player_ship = Player_Get_Ship(player, ship->type);
      if (player_ship)
      {
         *player_ship = *ship;
         status = Grid_Place_Ship(&player->grid, ship);
      }
   }
   return status;
}

static Ship_t * Player_Get_Ship(Player_t *player, Ship_Type_t type)
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
