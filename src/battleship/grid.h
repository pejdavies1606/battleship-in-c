/*
 * grid.h
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_GRID_H_
#define BATTLESHIP_GRID_H_

#include "commondefs.h"

typedef enum
{
   STATE_BLANK,
   STATE_HIT,
   STATE_MISS
} Hit_State_t;

typedef struct
{
   Ship_Type_t ship_type;
   Hit_State_t hit_state;
} Grid_State_t;

typedef struct
{
   Grid_State_t *defense;
   Hit_State_t *offense;
   size_t rows;
   size_t cols;
} Grid_t;

typedef enum
{
   GRID_STATUS_OK          = 0x0,
   GRID_STATUS_BORDER      = 0x1,
   GRID_STATUS_COLLISION   = 0x2,
   GRID_STATUS_UNKNOWN     = 0xF
} Grid_Status_t;

typedef struct
{
   size_t col_width;
   String_t corner_str;
   size_t corner_len;
   String_t side_str;
   size_t side_len;
} Grid_Meta_t;

void Grid_Meta_Init(Grid_Meta_t* meta, size_t col_size);
Grid_State_t * Grid_Init_Defense(size_t row_size, size_t col_size);
Hit_State_t * Grid_Init_Offense(size_t row_size, size_t col_size);
Grid_Status_t Grid_Place_Ship(const Grid_t *grid, const Ship_t *ship);

#endif /* BATTLESHIP_GRID_H_ */
