/*
 * player.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/player.h"

static Ship_t* Player_Get_Ship(Player_t *player, Ship_Type_e type);

Player_t * Player_Init(uint num_players)
{
   Player_t *players = malloc(num_players * sizeof(Player_t));
   if (players)
   {
      memset(players, 0, num_players * sizeof(Player_t));
      for (uint p = 0; p < num_players; p++)
      {
         Player_t *player = &players[p];
         if (Grid_Init(&player->grid, GRID_SIZE, GRID_SIZE))
         {
            player->ships = Ship_Init(NUM_SHIPS);
            if (player->ships)
            {
               for (uint i = 0; i < NUM_SHIPS; i++)
               {
                  player->ships[i].type = shipTable[i].type;
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

Grid_Status_e Player_Place_Ship(Player_t *player, Ship_t *ship)
{
  Grid_Status_e status = GRID_STATUS_NULL;
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

bool Player_Place_Ships_Auto(Player_t *player)
{
   Ship_t ship = {0};
   Grid_Status_e status = GRID_STATUS_UNKNOWN;
   bool result = false;
   if (player)
   {
      Grid_Clear_Defense(&player->grid);
      for (uint i = NUM_SHIPS; i > 0; i--)
      {
         status = GRID_STATUS_UNKNOWN;
         do
         {
            ship.type = shipTable[i - 1].type;
            ship.location = Coord_Init_Random(
                0, (int)player->grid.rows,
                0, (int)player->grid.cols);
            ship.heading = Heading_Init_Random();
            status = Player_Place_Ship(player, &ship);
         } while (
             GRID_STATUS_BORDER & status ||
             GRID_STATUS_COLLISION & status);
         if (GRID_STATUS_NULL == status)
         {
            break;
         }
      }
      result = true;
   }
   return result;
}

static Ship_t * Player_Get_Ship(Player_t *player, Ship_Type_e type)
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
