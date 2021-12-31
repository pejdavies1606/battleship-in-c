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
} Hit_State_e;

typedef struct
{
   Ship_Type_e ship_type;
   Hit_State_e hit_state;
} Grid_State_t;

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
   Ship_Type_e *ships;
   Hit_State_e *hits;
   uint rows;
   uint cols;
   Grid_Meta_t meta;
} Grid_t;

const char * Grid_GetHitStateStr(Hit_State_e const state);

bool Grid_InitMeta(Grid_Meta_t* meta,
		size_t row_size, size_t col_size,
		size_t row_width, size_t col_width);
bool Grid_IsValidMeta(Grid_Meta_t const * const meta);

bool Grid_Init(Grid_t *grid, uint rows, uint cols);
bool Grid_InitShips(Grid_t *grid);
bool Grid_InitHits(Grid_t *grid);

bool Grid_ClearShips(const Grid_t *grid);
bool Grid_ClearHits(const Grid_t *grid);

bool Grid_GetRowStr(
    Grid_t const *const grid,
    bool showOffense,
    int row,
    char *const line,
    size_t line_size,
    size_t *line_pos);

Grid_Status_e Grid_PlaceShip(const Grid_t *grid, const Ship_t *ship);

bool Ship_TypeToStr(const Ship_Type_e type, char *str, const size_t str_len);
bool Grid_HitStateToStr(const Hit_State_e state, char *str, size_t str_len);

#endif /* BATTLESHIP_GRID_H_ */
