#include "battleship/comp_player.h"
#include "battleship/player.h"
#include "battleship/input.h"

#include "battleship/ui.h"

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
   Search_t *const search);

bool _GetCoordLine(
   GridState_e const *const states,
   Coord_t *const target,
   Search_t *const search);

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
      switch (comp->state)
      {
      case SEARCH_RANDOM:
         if (comp->hit)
         {
            // TODO check search for existing hits and goto line
            comp->state = SEARCH_CIRCLE;
            comp->search.heading = Heading_InitRandom();
            int i = rand() % 2;
            debug_print("%d\n", i);
            comp->search.direction = (1 == i);
            comp->search.tries = NUM_HEADINGS - 1;
         }
         result = true;
         break;
      case SEARCH_CIRCLE:
         if (comp->hit)
         {
            comp->state = SEARCH_LINE;
            comp->search.tries = 2;
            comp->search.pos = 2;
         }
         else
         {
            if (0 == comp->search.tries)
            {
               comp->state = SEARCH_RANDOM;
            }
            else
            {
               comp->search.heading = Heading_GetNext(
                   comp->search.heading,
                   comp->search.direction);
            }
         }
         result = true;
         break;
      case SEARCH_LINE:
         if (!comp->hit)
         {
            if (0 == comp->search.tries)
            {
               comp->state = SEARCH_RANDOM;
            }
            else
            {
               // return to centre
               comp->search.pos = 1;
               // rotate twice to get opposite direction
               comp->search.heading = Heading_GetNext(
                   comp->search.heading,
                   comp->search.direction);
               comp->search.heading = Heading_GetNext(
                   comp->search.heading,
                   comp->search.direction);
            }
         }
         else
         {
            // extend range
            comp->search.pos++;
            // continue in same direction
            comp->search.tries++;
         }
         result = true;
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
      switch(comp->state)
      {
         case SEARCH_RANDOM:
            result = _GetCoordRandom(
               states,
               target);
            if (result)
            {
               comp->search.centre = *target;
            }
            break;
      case SEARCH_CIRCLE:
            result = _GetCoordCircle(
               states,
               target,
               &comp->search);
            break;
      case SEARCH_LINE:
            result = _GetCoordLine(
               states,
               target,
               &comp->search);
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
         result = BattleShipUI_ReadCoord(target);
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
   Search_t *const search)
{
   bool result = false;
   if (states && target && search)
   {
      while (!result && search->tries > 0)
      {
         Ship_t ship = {0};
         ship.location = search->centre;
         ship.heading = search->heading;
         *target = Ship_GetPoint(&ship, 1);
         bool row = ValidateRange(
             target->row,
             0,
             MAX_COORD_ROW);
         bool col = ValidateRange(
             target->col,
             0,
             MAX_COORD_COL);
         result = (row && col);
         search->tries--;
         debug_print(
            "%u (%d,%d) %d\n",
            search->tries,
            target->row,
            target->col,
            result);
         if (!result)
         {
            search->heading = Heading_GetNext(
                search->heading,
                search->direction);
         }
      }
   }
   return result;
}

bool _GetCoordLine(
   GridState_e const *const states,
   Coord_t *const target,
   Search_t *const search)
{
   bool result = false;
   if (states && target && search)
   {
      while (!result && search->tries > 0)
      {
         Ship_t ship = {0};
         ship.location = search->centre;
         ship.heading = search->heading;
         *target = Ship_GetPoint(&ship, search->pos);
         bool row = ValidateRange(
             target->row,
             0,
             MAX_COORD_ROW);
         bool col = ValidateRange(
             target->col,
             0,
             MAX_COORD_COL);
         result = (row && col);
         search->tries--;
         debug_print(
            "%u (%d,%d) %d\n",
            search->tries,
            target->row,
            target->col,
            result);
         if (!result)
         {
            // return to centre
            search->pos = 1;
            // rotate twice to get opposite direction
            search->heading = Heading_GetNext(
                search->heading,
                search->direction);
            search->heading = Heading_GetNext(
                search->heading,
                search->direction);
         }
      }
   }
   return result;
}