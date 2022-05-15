#include "battleship/comp_player.h"

bool BattleShipAI_GetCoord(
   Comp_Player_t *const comp,
   Coord_t *const target)
{
   bool result = false;
   if (comp && target)
   {
      switch(comp->search)
      {
         case SEARCH_RANDOM:
            *target = Coord_InitRandom(
                0, MAX_COORD_ROW,
                0, MAX_COORD_COL);
            result = true;
            break;
         default:
            break;
      }
   }
   return result;
}