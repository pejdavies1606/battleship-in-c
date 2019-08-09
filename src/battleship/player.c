/*
 * player.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "player.h"

Grid_State_t Grid_State_Init(const Ship_Type_t type, const Hit_State_t state)
{
   Grid_State_t grid_state =
   {
         .ship_type = type,
         .hit_state = state
   };

   return grid_state;
}

Player_t Player_Init()
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif

   Grid_State_t *defense = malloc(GRID_SIZE * GRID_SIZE * sizeof(Grid_State_t));
   Hit_State_t *offense = malloc(GRID_SIZE * GRID_SIZE * sizeof(Hit_State_t));

   for (uint row = 0; row < GRID_SIZE; row++)
   {
      for (uint col = 0; col < GRID_SIZE; col++)
      {
         defense[row*GRID_SIZE + col] = Grid_State_Init(SHIP_NONE, STATE_BLANK);
         offense[row*GRID_SIZE + col] = STATE_BLANK;
      }
   }

   Ship_t ships[NUM_SHIPS];
   Scoreboard_Entity_t ship_health[NUM_SHIPS];
   for (uint i = 0; i < NUM_SHIPS; i++)
   {
      ships[i] = Ship_Init( (Ship_Type_t) i );
      ship_health[i] = Scoreboard_Entity_Init(SHIP_NAME[i], 0, SHIP_LENGTH[i]);
   }

   Scoreboard_t scoreboard_ship_health;
   scoreboard_ship_health.num_entities = NUM_SHIPS;
   scoreboard_ship_health.entities = ship_health;

   Player_t player =
   {
         .defense = defense,
         .offense = offense,
         .ships = ships,
         .scoreboard_ship_health = scoreboard_ship_health
   };

   return player;
}
