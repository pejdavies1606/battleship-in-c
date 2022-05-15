/*
 * grid.h
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_GRID_H_
#define BATTLESHIP_GRID_H_

#include "battleship/commondefs.h"
#include "battleship/coord.h"
#include "battleship/ship.h"
#include "battleship/line.h"

#define SIZE_GRID (MAX_COORD_ROW * MAX_COORD_COL)

#define STR_GRID_SIDE_V "|"
#define STR_GRID_SIDE_H "="
#define STR_GRID_CORNER "+"
#define LEN_GRID_SPACE 1

#define LEN_GRID_CORNER (LEN_COORD_COL + LEN_GRID_SPACE)
#define LEN_GRID_SIDE ((LEN_COORD_COL + LEN_GRID_SPACE) * MAX_COORD_COL)

#define GRID_ROW_TITLE -2
#define GRID_ROW_HEADER -1

typedef enum GridState
{
   GRID_STATE_BLANK,
   GRID_STATE_MISS,
   GRID_STATE_HIT,
   GRID_STATE_SUNK,
} GridState_e;

typedef enum GridStatus
{
   GRID_STATUS_OK          = 0x0,
   GRID_STATUS_BORDER      = 0x1,
   GRID_STATUS_COLLISION   = 0x2,
   GRID_STATUS_UNKNOWN     = 0xF
} GridStatus_e;

typedef struct GridMeta
{
   size_t row_width;
   size_t col_width;
   char corner_str[LEN_GRID_CORNER + 1];
   char side_str[LEN_GRID_SIDE + 1];
} GridMeta_t;

typedef struct Grid
{
   ShipType_e ships[SIZE_GRID];
   GridState_e states[SIZE_GRID];
   GridMeta_t meta;
} Grid_t;

char const * Grid_GetStateStr(
   GridState_e const state);

bool Grid_Init(
   Grid_t * const grid);

bool Grid_Clear(
   Grid_t * const grid);

bool Grid_GetRowStr(
   Grid_t const * const def_grid,
   Line_t * const line,
   int const row,
   bool const off_grid);

bool Grid_PlaceShip(
   Grid_t * const grid,
   Ship_t * const player_ship,
   const Ship_t * const ship,
   GridStatus_e * const status);

bool Grid_PlaceHit(
   Grid_t const * const grid,
   GridState_e * const states,
   Coord_t const * const target,
   ShipType_e * const hit_ship);

bool Grid_SinkShip(
   const Grid_t * const grid,
   GridState_e * const states,
   Ship_t const * const ship);

bool Grid_StateToStr(
   GridState_e const state,
   char const * const str,
   size_t const str_len);

bool Grid_ShipTypeToStr(
   ShipType_e const type,
   char const * const str,
   const size_t str_len);

#endif /* BATTLESHIP_GRID_H_ */
