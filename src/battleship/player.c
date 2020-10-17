/*
 * player.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "player.h"

static Ship_t* Player_Get_Ship(Player_t *player, Ship_Type_t type);

Player_t Player_Init(void)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   Ship_t *ships =
      malloc(NUM_SHIPS * sizeof(Ship_t));
   memset(ships, 0, NUM_SHIPS * sizeof(Ship_t));
   Scoreboard_Entity_t *ship_health =
      malloc(NUM_SHIPS * sizeof(Scoreboard_Entity_t));
   memset(ships, 0, NUM_SHIPS * sizeof(Scoreboard_Entity_t));
   for (uint i = 0; i < NUM_SHIPS; i++)
   {
      const Ship_Info_t ship_info = shipTable[i];
      ships[i].type = ship_info.type;
      ship_health[i] = Scoreboard_Entity_Init(
         ship_info.name, 0, ship_info.length);
   }

   Scoreboard_t scoreboard_ship_health =
   {
      .num_entities = NUM_SHIPS,
      .entities = ship_health
   };

   Grid_t grid =
   {
      .defense = Grid_Init_Defense(GRID_SIZE, GRID_SIZE),
      .offense = Grid_Init_Offense(GRID_SIZE, GRID_SIZE),
      .rows = GRID_SIZE,
      .cols = GRID_SIZE,
   };

   Player_t player =
   {
      .grid = grid,
      .ships = ships,
      .scoreboard_ship_health = scoreboard_ship_health
   };

   return player;
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
