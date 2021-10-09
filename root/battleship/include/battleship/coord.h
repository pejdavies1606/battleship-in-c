/*
 * coord.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef COORD_H_
#define COORD_H_

#include <string.h>
#include "battleship/commondefs.h"

typedef struct
{
   int row;
   int col;
} Coord_t;

typedef enum
{
   HEADING_UNKNOWN,
   HEADING_NORTH,
   HEADING_EAST,
   HEADING_SOUTH,
   HEADING_WEST
} Heading_e;

typedef struct
{
   Heading_e h;
   char c;
} HeadingInfo_t;

#define LEN_HEADING 1
#define NUM_HEADINGS 5
#define HEADING_UNKNOWN_INDEX 0
static const HeadingInfo_t headingTable[NUM_HEADINGS] =
{
   { HEADING_UNKNOWN, '\0'},
   { HEADING_NORTH,   'N' },
   { HEADING_EAST,    'E' },
   { HEADING_SOUTH,   'S' },
   { HEADING_WEST,    'W' }
};

static inline char Heading_Get_Char(Heading_e h)
{
   char c = headingTable[HEADING_UNKNOWN_INDEX].c;
   for (uint i = 0; i < NUM_HEADINGS; i++)
   {
      if (h == headingTable[i].h)
      {
         c = headingTable[i].c;
      }
   }
   return c;
}

static inline Heading_e Heading_Get_Hdg(char c)
{
   Heading_e h = headingTable[HEADING_UNKNOWN_INDEX].h;
   for (uint i = 0; i < NUM_HEADINGS; i++)
   {
      if (c == headingTable[i].c)
      {
         h = headingTable[i].h;
      }
   }
   return h;
}

static inline Coord_t Coord_Init(int row, int col)
{
   Coord_t coord =
   {
      .row = row,
      .col = col
   };
   return coord;
}

Coord_t Coord_Init_Random(
   int row_min, int row_max,
   int col_min, int col_max);

Heading_e Heading_Init_Random();

bool Coord_ColToChar(int col, char *c);
bool Coord_ColFromChar(char c, int *col);
bool Coord_RowFromStr(const char *str, int *row);

#endif /* COORD_H_ */
