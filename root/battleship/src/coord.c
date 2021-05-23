/*
 * coord.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/coord.h"

Coord_t Coord_Init_Random(
   int row_min, int row_max,
   int col_min, int col_max
)
{
   int row_diff = row_max - row_min;
   int col_diff = col_max - col_min;
   if (row_diff < 0 || col_diff < 0)
   {
      return Coord_Init(0, 0);
   }
   else
   {
      int row_rand = rand() % row_diff + row_min;
      int col_rand = rand() % col_diff + col_min;
      return Coord_Init(row_rand, col_rand);
   }
}

Heading_e Heading_Init_Random()
{
   int i = rand() % (int) NUM_HEADINGS;
   return headingTable[i].hdg;
}
