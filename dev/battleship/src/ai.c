#include "battleship/comp_player.h"
#include "battleship/player.h"

bool _ChangeState(
   Comp_Player_t *const comp);

bool _ActionState(
   Comp_Player_t *const comp,
   GridState_e const *const states,
   Coord_t *const target);

bool _GetCoordRandom(
   GridState_e const *const states,
   Coord_t *const target);

bool _GetCoordCircle(
   GridState_e const *const states,
   Coord_t *const target,
   CircleSearch_t *const circle);

bool BattleShipAI_GetCoord(
   Player_t *const player,
   GridState_e const *const states,
   Coord_t *const target)
{
   bool result = false;
   if (player && states && target)
   {
      result = _ChangeState(&player->comp);
      if (result)
      {
         result = _ActionState(
             &player->comp,
             states,
             target);
      }
   }
   return result;
}

bool _ChangeState(Comp_Player_t *const comp)
{
   bool result = false;
   if (comp)
   {
      switch (comp->search)
      {
      case SEARCH_RANDOM:
         if (comp->hit)
         {
            //comp->search = SEARCH_CIRCLE;
            comp->circle.heading = Heading_InitRandom();
            int i = rand() % 2;
            comp->circle.clockwise = (0 == i);
            comp->circle.tries = NUM_HEADINGS - 1;
         }
         result = true;
         break;
      case SEARCH_CIRCLE:
         if (comp->hit)
         {
            comp->search = SEARCH_LINE;
         }
         else
         {
            if (0 == comp->circle.tries)
            {
               comp->search = SEARCH_RANDOM;
            }
            else
            {
               comp->circle.heading = Heading_GetNext(
                   comp->circle.heading,
                   comp->circle.clockwise);
            }
         }
         result = true;
         break;
      case SEARCH_LINE:
         if (!comp->hit)
         {
            // TODO
         }
         //result = true;
      default:
         // result initialised to false
         break;
      }
   }
   return result;
}

bool _ActionState(
   Comp_Player_t *const comp,
   GridState_e const *const states,
   Coord_t *const target)
{
   bool result = false;
   if (comp && states && target)
   {
      switch(comp->search)
      {
         case SEARCH_RANDOM:
            result = _GetCoordRandom(
               states,
               target);
            break;
      case SEARCH_CIRCLE:
            result = _GetCoordCircle(
               states,
               target,
               &comp->circle);
            break;
         default:
            break;
      }
   }
   return result;
}

bool _GetCoordRandom(
   GridState_e const *const states,
   Coord_t *const target)
{
   bool result = false;
   if (states && target)
   {
      uint retry = 0U;
      do
      {
#ifdef DEBUG
         static uint i = 0U;
         #define NUM_COORDS 2
         static Coord_t COORD_TABLE[NUM_COORDS] =
             {
                 {1, 7},
                 {2, 7},
             };
         if (i < NUM_COORDS)
         {
            *target = COORD_TABLE[i++];
         }
#else
         *target = Coord_InitRandom(
             0, MAX_COORD_ROW,
             0, MAX_COORD_COL);
#endif
         result =
             (GRID_STATE_BLANK ==
              states[target->col + target->row * MAX_COORD_COL]);
      } while (!result && ++retry < SIZE_GRID);
   }
   return result;
}

bool _GetCoordCircle(
   GridState_e const *const states,
   Coord_t *const target,
   CircleSearch_t *const circle)
{
   bool result = false;
   if (states && target && circle)
   {
      // TODO
   }
   return result;
}