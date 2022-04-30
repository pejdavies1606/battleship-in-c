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