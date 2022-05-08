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
} ShipType_e;

typedef struct
{
   ShipType_e type;
   Coord_t location;
   Heading_e heading;
   uint hits;
   bool sunk;
} Ship_t;

typedef struct
{
   ShipType_e type;
   char * name;
   uint length;
} Ship_Info_t;

#define NUM_SHIPS 5
#define LEN_SHIPS 1

static Ship_Info_t const SHIP_TABLE[NUM_SHIPS] =
{
   { SHIP_DESTROYER,          "Destroyer",         2 },
   { SHIP_CRUISER,            "Cruiser",           3 },
   { SHIP_SUBMARINE,          "Submarine",         3 },
   { SHIP_BATTLESHIP,         "Battleship",        4 },
   { SHIP_AIRCRAFT_CARRIER,   "Aircraft Carrier",  5 }
};

static inline Ship_Info_t const * Ship_GetInfo(const ShipType_e type)
{
   Ship_Info_t const * info = NULL;
   for (uint i = 0; i < NUM_SHIPS; i++)
   {
      if (type == SHIP_TABLE[i].type)
      {
         info = &SHIP_TABLE[i];
         break;
      }
   }
   return info;
}

Coord_t Ship_GetPoint(Ship_t const * const ship, uint const i);

bool Ship_Hit(Ship_t * const ship, bool *const sunk);

#endif /* BATTLESHIP_SHIP_H_ */