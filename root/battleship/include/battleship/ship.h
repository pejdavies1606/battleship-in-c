/*
 * ship.h
 *
 *  Created on: 1 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_SHIP_H_
#define BATTLESHIP_SHIP_H_

#include "battleship/commondefs.h"
#include "battleship/coord.h"

typedef enum
{
   SHIP_NONE,
   SHIP_DESTROYER,
   SHIP_CRUISER,
   SHIP_SUBMARINE,
   SHIP_BATTLESHIP,
   SHIP_AIRCRAFT_CARRIER
} Ship_Type_t;

typedef struct
{
   Ship_Type_t type;
   Coord_t location;
   Heading_t heading;
   uint hits;
} Ship_t;

typedef struct
{
   Ship_Type_t type;
   String_t name;
   uint length;
} Ship_Info_t;

static const Ship_Info_t shipTable[] =
{
   { SHIP_DESTROYER,          "Destroyer",         2 },
   { SHIP_CRUISER,            "Cruiser",           3 },
   { SHIP_SUBMARINE,          "Submarine",         3 },
   { SHIP_BATTLESHIP,         "Battleship",        4 },
   { SHIP_AIRCRAFT_CARRIER,   "Aircraft Carrier",  5 }
};

#define NUM_SHIPS ARRAY_LEN(shipTable)

static inline const Ship_Info_t * Ship_Get_Info(const Ship_Type_t type)
{
   for (uint i = 0; i < NUM_SHIPS; i++)
   {
      if (type == shipTable[i].type)
      {
         return &shipTable[i];
      }
   }
   return NULL;
}

static inline const uint * Ship_Get_Length_Array(void)
{
   uint *ship_length = malloc(sizeof(uint) * NUM_SHIPS);
   for (uint i = 0; i < NUM_SHIPS; i++)
   {
      ship_length[i] = shipTable[i].length;
   }
   return ship_length;
}

static inline Coord_t Ship_Get_Point(const Ship_t *ship, const uint i)
{
   Coord_t point = Coord_Init(0, 0);
   if (ship)
   {
      point = ship->location;
      switch (ship->heading)
      {
      case HEADING_NORTH:
         point.row -= (int) i;
         break;
      case HEADING_EAST:
         point.col += (int) i;
         break;
      case HEADING_SOUTH:
         point.row += (int) i;
         break;
      case HEADING_WEST:
         point.col -= (int) i;
         break;
      }
   }
   return point;
}

Ship_t * Ship_Init(uint num_ships);

#endif /* BATTLESHIP_SHIP_H_ */
