/*
 * grid.h
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_GRID_H_
#define BATTLESHIP_GRID_H_

#include "commondefs.h"

typedef struct
{
   size_t col_width;
   String_t corner_str;
   size_t corner_len;
   String_t side_str;
   size_t side_len;
} Grid_Meta_t;

void Grid_Meta_Init(Grid_Meta_t* meta, size_t grid_size);

#endif
