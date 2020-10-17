/*
 * coord.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef COORD_H_
#define COORD_H_

#include <string.h>
#include "commondefs.h"
#include "input.h"

typedef struct
{
   int row;
   int col;
} Coord_t;

typedef enum
{
   HEADING_NORTH,
   HEADING_EAST,
   HEADING_SOUTH,
   HEADING_WEST
} Heading_t;

typedef struct
{
   Heading_t hdg;
   String_t str;
} HeadingInfo_t;

static const HeadingInfo_t headingTable[] =
{
   { HEADING_NORTH, "N" },
   { HEADING_EAST,  "E" },
   { HEADING_SOUTH, "S" },
   { HEADING_WEST,  "W" }
};

#define NUM_HEADINGS ARRAY_LEN(headingTable)
#define LEN_HEADING 1

static inline String_t Heading_Get_Str(Heading_t hdg)
{
   for (uint i = 0; i < NUM_HEADINGS; i++)
   {
      if (hdg == headingTable[i].hdg)
      {
         return headingTable[i].str;
      }
   }
   return NULL;
}

static inline bool ValidateHeading(const String_t input)
{
   if (!input || strlens(input) > LEN_HEADING)
   {
      return false;
   }
   for (uint i = 0; i < NUM_HEADINGS; i++)
   {
      if (0 == strncmp(input, headingTable[i].str, LEN_HEADING))
      {
         return true;
      }
   }
   return false;
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

Heading_t Heading_Init_Random();

#endif /* COORD_H_ */
