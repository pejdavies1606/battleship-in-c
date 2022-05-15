#include "battleship/comp_player.h"
#include "battleship/player.h"

bool _GetCoord(
   GridState_e const *const states,
   Coord_t *const target);

bool BattleShipAI_GetCoord(
   Player_t *const player,
   GridState_e const *const states,
   Coord_t *const target)
{
   bool result = false;
   if (player && target)
   {
      switch(player->comp.search)
      {
         case SEARCH_RANDOM:
            result = _GetCoord(
               states,
               target);
            break;
         default:
            break;
      }
   }
   return result;
}

bool _GetCoord(
   GridState_e const *const states,
   Coord_t *const target)
{
   bool result = false;
   if (target)
   {
      uint retry = 0U;
      do
      {
         *target = Coord_InitRandom(
             0, MAX_COORD_ROW,
             0, MAX_COORD_COL);
         result =
             (GRID_STATE_BLANK ==
              states[target->col + target->row * MAX_COORD_COL]);
      } while (!result && ++retry < SIZE_GRID);
   }
   return result;
}