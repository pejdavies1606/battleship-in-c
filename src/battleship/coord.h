/*
 * coord.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef _COORD_H_
#define _COORD_H_

#include <string.h>
#include "commondefs.h"
#include "input.h"

#define INIT_ROW 0
#define INIT_COL 0

typedef struct
{
   int row;
   int col;
} Coord_t;

typedef enum
{
   NORTH,
   EAST,
   SOUTH,
   WEST
} Heading_t;

typedef struct
{
   Heading_t hdg;
   String_t str;
} HeadingInfo_t;

static const HeadingInfo_t headingTable[] =
{
   { NORTH, "N" },
   { EAST,  "E" },
   { SOUTH, "S" },
   { WEST,  "W" }
};

#define NUM_HEADINGS ARRAY_LEN(headingTable)
#define LEN_HEADING 1

static inline const String_t Heading_Get_Str(Heading_t hdg)
{
   uint i;
   for (i = 0; i < NUM_HEADINGS; i++)
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
   uint i;
   for (i = 0; i < NUM_HEADINGS; i++)
   {
      if (0 == strncmp(input, headingTable[i].str, LEN_HEADING))
      {
         return true;
      }
   }
   return false;
}

Coord_t Coord_Init();

#endif /* _COORD_H_ */
