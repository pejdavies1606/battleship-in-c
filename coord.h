/*
 * coord.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef COORD_H_
#define COORD_H_


#include "commondefs.h"

#define INIT_ROW 0
#define INIT_COL 0

typedef struct
{
   uint row;
   uint col;
} Coord_t;

typedef enum
{
   NORTH,
   EAST,
   SOUTH,
   WEST
} Heading_t;

Coord_t Coord_Init();

#endif /* COORD_H_ */
