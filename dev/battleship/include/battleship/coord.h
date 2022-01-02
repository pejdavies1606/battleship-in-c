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

#define COORD_ROW_MAX 10
#define COORD_COL_MAX 10

typedef struct Coord
{
   int row;
   int col;
} Coord_t;

typedef enum Heading
{
   HEADING_UNKNOWN,
   HEADING_NORTH,
   HEADING_EAST,
   HEADING_SOUTH,
   HEADING_WEST
} Heading_e;

typedef struct HeadingInfo
{
   Heading_e h;
   char c;
} HeadingInfo_t;

#define LEN_HEADING 1
#define NUM_HEADINGS 5
static const HeadingInfo_t HEADING_TABLE[NUM_HEADINGS] =
{
   { HEADING_NORTH,   'N' },
   { HEADING_EAST,    'E' },
   { HEADING_SOUTH,   'S' },
   { HEADING_WEST,    'W' },
   { HEADING_UNKNOWN, '0' }
};

static inline char Heading_GetChar(Heading_e h)
{
   char c = HEADING_TABLE[NUM_HEADINGS - 1].c;
   for (uint i = 0; i < NUM_HEADINGS; i++)
   {
      if (h == HEADING_TABLE[i].h)
      {
         c = HEADING_TABLE[i].c;
      }
   }
   return c;
}

static inline Heading_e Heading_GetHdg(char c)
{
   Heading_e h = HEADING_TABLE[NUM_HEADINGS - 1].h;
   for (uint i = 0; i < NUM_HEADINGS; i++)
   {
      if (c == HEADING_TABLE[i].c)
      {
         h = HEADING_TABLE[i].h;
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

Coord_t Coord_InitRandom(
   int row_min, int row_max,
   int col_min, int col_max);

Heading_e Heading_InitRandom();

bool Coord_ColToChar(int col, char *c);
bool Coord_ColFromChar(char c, int *col);
bool Coord_RowFromStr(const char *str, int *row);

#endif /* COORD_H_ */
