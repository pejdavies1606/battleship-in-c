/*
 * coord.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "coord.h"

Coord_t Coord_Init()
{
   Coord_t coord =
   {
         .row = INIT_ROW,
         .col = INIT_COL
   };
   return coord;
}
