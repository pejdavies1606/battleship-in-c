/*
 * coord.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/coord.h"
#include "battleship/input.h"

#include <ctype.h>

Coord_t Coord_Init_Random(
   int row_min, int row_max,
   int col_min, int col_max)
{
   Coord_t c = { 0 };
   int row_diff = row_max - row_min;
   int col_diff = col_max - col_min;
   int row_rand = 0;
   int col_rand = 0;
   if (row_diff >= 0 && col_diff >= 0)
   {
      row_rand = rand() % row_diff + row_min;
      col_rand = rand() % col_diff + col_min;
      c.row = row_rand;
      c.col = col_rand;
   }
   return c;
}

Heading_e Heading_Init_Random()
{
   int i = rand() % (int) NUM_HEADINGS - 1;
   return headingTable[i + 1].h;
}

bool Coord_ColToChar(int col, char *c)
{
   bool result = false;
   if (c && col >= 0 && col < COORD_COL_MAX)
   {
      *c = (char) (col + 'A'); // ASCII
      result = true;
   }
   return result;
}

bool Coord_ColFromChar(char c, int *col)
{
   bool result = false;
   if (col)
   {
      c = (char) toupper(c);
      if (c >= 'A' && c <= 'Z')
      {
         *col = (int) c - 'A'; // ASCII
         result = true;
      }
   }
   return result;
}

bool Coord_RowFromStr(const char *str, int *row)
{
   bool result = false;
   int rowval = 0;
   if (row)
   {
      if (ParseInt(str, &rowval) &&
         rowval > 0 &&
         rowval <= COORD_ROW_MAX)
      {
         *row = rowval - 1;
         result = true;
      }
   }
   return result;
}
