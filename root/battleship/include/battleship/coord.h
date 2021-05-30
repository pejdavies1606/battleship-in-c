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
#include "battleship/input.h"

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
} Heading_e;

typedef struct
{
   Heading_e hdg;
   String_t str;
} HeadingInfo_t;

#define LEN_HEADING 1
#define NUM_HEADINGS 4
static const HeadingInfo_t headingTable[NUM_HEADINGS] =
{
   { HEADING_NORTH, "N" },
   { HEADING_EAST,  "E" },
   { HEADING_SOUTH, "S" },
   { HEADING_WEST,  "W" }
};

static inline String_t Heading_Get_Str(Heading_e hdg)
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

static inline bool Validate_Heading(const String_t input, void *output)
{
   bool result = false;
   uint *heading = (uint*) output; // Heading_e might not be same size as uint
   if (input && output && strlens(input) <= LEN_HEADING)
   {
      for (uint i = 0; i < NUM_HEADINGS; i++)
      {
         if (0 == strncmp(input, headingTable[i].str, LEN_HEADING))
         {
            *heading = (uint) headingTable[i].hdg;
            result = true;
            break;
         }
      }
   }
   return result;
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

#endif /* COORD_H_ */
