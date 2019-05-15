/*
 * ship.h
 *
 *  Created on: 1 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_SHIP_H_
#define BATTLESHIP_SHIP_H_

#include "commondefs.h"
#include "coord.h"

// Ship             Length
// Destroyer        2
// Cruiser          3
// Submarine        3
// Battleship       4
// Aircraft Carrier 5

// must match enum below
#define NUM_SHIPS 5

typedef enum
{
   DESTROYER,
   CRUISER,
   SUBMARINE,
   BATTLESHIP,
   AIRCRAFT_CARRIER
} Ship_Type_t;

// ensure these arrays correspond with the enum above
extern const String_t SHIP_NAME[NUM_SHIPS];
extern const uint SHIP_LENGTH[NUM_SHIPS];

typedef struct
{
   Ship_Type_t type;
   Coord_t location;
   Heading_t heading;
   uint hits;
} Ship_t;

Ship_t Ship_Init( Ship_Type_t type );

#endif /* BATTLESHIP_SHIP_H_ */
