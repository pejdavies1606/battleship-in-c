/*
 * ship.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "ship.h"

// see ship.h
const String_t SHIP_NAME[] =
{
      "Destroyer",
      "Cruiser",
      "Submarine",
      "Battleship",
      "Aircraft Carrier"
};
const uint SHIP_LENGTH[] =
{
      2,
      3,
      3,
      4,
      5
};

Ship_t Ship_Init( Ship_Type_t type )
{
   Ship_t ship =
   {
         .type = type,
         .location  = Coord_Init(),
         .heading = NORTH,
         .hits = 0
   };
   return ship;
}
