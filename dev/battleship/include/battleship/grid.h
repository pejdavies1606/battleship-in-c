/*
 * grid.h
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_GRID_H_
#define BATTLESHIP_GRID_H_

#include "battleship/commondefs.h"

#define GRID_ROW_TITLE -2
#define GRID_ROW_HEADER -1

typedef enum
{
   HIT_STATE_BLANK,
   HIT_STATE_HIT,
   HIT_STATE_MISS
} GridHit_e;

typedef enum
{
   GRID_STATUS_OK          = 0x0,
   GRID_STATUS_BORDER      = 0x1,
   GRID_STATUS_COLLISION   = 0x2,
   GRID_STATUS_NULL        = 0xE,
   GRID_STATUS_UNKNOWN     = 0xF
} Grid_Status_e;

typedef struct
{
   size_t row_width;
   size_t col_width;
   String_t corner_str;
   size_t corner_len;
   String_t side_str;
   size_t side_len;
} Grid_Meta_t;

typedef struct
{
   ShipType_e *ships;
   GridHit_e *hits;
   uint rows;
   uint cols;
   Grid_Meta_t meta;
} Grid_t;

const char * Grid_GetHitStateStr(GridHit_e const state);

bool Grid_InitMeta(Grid_Meta_t* meta,
		size_t row_size, size_t col_size,
		size_t row_width, size_t col_width);
bool Grid_IsValidMeta(Grid_Meta_t const * const meta);

bool Grid_Init(Grid_t * const grid, uint const rows, uint const cols);
ShipType_e * Grid_InitShips(uint rows, uint cols);
GridHit_e * Grid_InitHits(uint rows, uint cols);

bool Grid_ClearShips(ShipType_e * const ships, uint const rows, uint const cols);
bool Grid_ClearHits(GridHit_e * const hits, uint const rows, uint const cols);

bool Grid_GetRowStr(
    Grid_t *const grid,
    GridHit_e * const hits,
    int const row,
    char * const line,
    size_t const line_size,
    size_t * const line_pos);

Grid_Status_e Grid_PlaceShip(const Grid_t *grid, const Ship_t *ship);

bool Ship_TypeToStr(const ShipType_e type, char *str, const size_t str_len);
bool Grid_HitStateToStr(const GridHit_e state, char *str, size_t str_len);

#endif /* BATTLESHIP_GRID_H_ */
