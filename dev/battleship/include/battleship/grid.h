/*
 * grid.h
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_GRID_H_
#define BATTLESHIP_GRID_H_

#include "battleship/commondefs.h"

#define SIZE_GRID (MAX_COORD_ROW * MAX_COORD_COL)

#define GRID_ROW_TITLE -2
#define GRID_ROW_HEADER -1

typedef enum GridState
{
   GRID_STATE_BLANK,
   GRID_STATE_HIT,
   GRID_STATE_MISS
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
   char * corner_str;
   size_t corner_len;
   char * side_str;
   size_t side_len;
} GridMeta_t;

typedef struct Grid
{
   ShipType_e ships[SIZE_GRID];
   GridState_e states[SIZE_GRID];
   uint rows;
   uint cols;
   GridMeta_t meta;
} Grid_t;

char const * Grid_GetStateStr(
   GridState_e const state);

bool Grid_Init(
   Grid_t * const grid);

void Grid_Destroy(
   Grid_t * const grid);

bool Grid_Clear(
   Grid_t * const grid);

bool Grid_GetRowStr(
   Grid_t const * const grid,
   GridState_e const * const states,
   int const row,
   char * const line,
   size_t const line_size,
   size_t * const line_pos);

bool Grid_PlaceShip(
   Grid_t * const grid,
   Ship_t * const player_ship,
   const Ship_t * const ship,
   GridStatus_e * const status);

bool Grid_PlaceHit(
   Grid_t const * const grid,
   GridState_e * const states,
   Coord_t const * const location,
   bool * const hit);

bool Grid_StateToStr(
   GridState_e const state,
   char const * const str,
   size_t const str_len);

bool Grid_ShipTypeToStr(
   ShipType_e const type,
   char const * const str,
   const size_t str_len);

#endif /* BATTLESHIP_GRID_H_ */
