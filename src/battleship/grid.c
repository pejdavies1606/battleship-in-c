/*
 * grid.c
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#include "ui.h"
#include "util.h"
#include "grid.h"

void Grid_Meta_Init(Grid_Meta_t* meta, size_t grid_size)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   meta->col_width = CalcNumWidth(grid_size);

   meta->corner_len = meta->col_width + SIZE_GRID_SPACE;
   String_t corner_char = STR_GRID_CORNER;
   meta->corner_str = malloc(meta->corner_len);
   RepeatChar(meta->corner_str, meta->corner_len, corner_char[0]);

   meta->side_len = (meta->col_width + SIZE_GRID_SPACE)*grid_size;
   String_t side_char = STR_GRID_SIDE_H;
   meta->side_str = malloc(meta->side_len);
   RepeatChar(meta->side_str, meta->side_len, side_char[0]);
}
