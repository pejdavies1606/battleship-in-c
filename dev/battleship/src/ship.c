/*
 * ship.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/ship.h"

Coord_t Ship_GetPoint(Ship_t const * const ship, uint const i)
{
   Coord_t point = { 0 };
   if (ship)
   {
      point = ship->location;
      switch (ship->heading)
      {
      case HEADING_NORTH:
         point.row += (int) i;
         break;
      case HEADING_EAST:
         point.col -= (int) i;
         break;
      case HEADING_SOUTH:
         point.row -= (int) i;
         break;
      case HEADING_WEST:
         point.col += (int) i;
         break;
      case HEADING_UNKNOWN:
         break;
      }
   }
   return point;
}

bool Ship_Hit(Ship_t * const ship, bool *const sunk)
{
   bool result = false;
   if (ship && sunk)
   {
      Ship_Info_t const * const ship_info = Ship_GetInfo(ship->type);
      if (ship_info)
      {
         ship->hits++;
         ship->sunk = (ship->hits == ship_info->length);
         *sunk = ship->sunk;
         result = true;
      }
   }
   return result;
}
