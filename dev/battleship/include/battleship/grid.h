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
   GRID_STATUS_NULL        = 0xE,
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
   ShipType_e *ships;
   GridState_e *states;
   uint rows;
   uint cols;
   GridMeta_t meta;
} Grid_t;

char const * Grid_GetStateStr(
   GridState_e const state);

bool Grid_Init(
   Grid_t * const grid,
   uint const rows,
   uint const cols);

void Grid_Destroy(
   Grid_t * const grid);

bool Grid_ClearShips(
   ShipType_e * const ships,
   uint const rows,
   uint const cols);

bool Grid_ClearStates(
   GridState_e * const states,
   uint const rows,
   uint const cols);

bool Grid_GetRowStr(
   Grid_t const * const grid,
   GridState_e const * const states,
   int const row,
   char * const line,
   size_t const line_size,
   size_t * const line_pos);

GridStatus_e Grid_PlaceShip(
   Grid_t const * const grid,
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
