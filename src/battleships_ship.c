/*
 * ship.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleships_ship.h"

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

/*
 * Ship_Init
 *
 * Returns the enum for the given index, or -1 if the index is out of range
 */
Ship_Type_t Ship_Init( uint i )
{
   Ship_Type_t type = -1;
   if ( i < NUM_SHIPS )
   {
      switch (i)
      {
         case 0:
            type = DESTROYER;
            break;
         case 1:
            type = CRUISER;
            break;
         case 2:
            type = SUBMARINE;
            break;
         case 3:
            type = BATTLESHIP;
            break;
         case 4:
            type = AIRCRAFT_CARRIER;
            break;
      }
   }
   return type;
}

Ship_Player_t Ship_Player_Init( uint i )
{
   Ship_Player_t ship =
   {
         .type = Ship_Init( i ),
         .location  = Coord_Init(),
         .heading = NORTH,
         .hits = 0
   };
   return ship;
}
