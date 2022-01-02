/*
 * grid.c
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#include "battleship/ui.h"
#include "battleship/util.h"
#include "battleship/grid.h"

typedef struct GridStateInfo
{
   GridState_e state;
   char const * str;
} GridStateInfo_t;

#define GRID_STATE_COUNT 3
static GridStateInfo_t const GRID_STATE_TABLE[GRID_STATE_COUNT] =
{
   { GRID_STATE_BLANK, " " },
   { GRID_STATE_HIT,   "x" },
   { GRID_STATE_MISS,  "o" },
};

const char * Grid_GetStateStr(GridState_e const state)
{
   char const * str = NULL;
   for (uint i = 0; i < GRID_STATE_COUNT; i++)
   {
      if (state == GRID_STATE_TABLE[i].state)
      {
         str = GRID_STATE_TABLE[i].str;
      }
   }
   return str;
}

static GridStatus_e Grid_CheckShip(
   const Grid_t *grid,
   const Ship_t *ship);

static bool Grid_AppendLine(
    char *const line,
    size_t const line_size,
    size_t *const line_pos,
    size_t const space_len,
    char *const str,
    size_t const str_size);

bool Grid_Init(
   Grid_t * const grid,
   uint const rows,
   uint const cols)
{
   bool result = false;
   if (grid)
   {
      grid->rows = rows;
      grid->cols = cols;
      grid->ships = Grid_InitShips(rows, cols);
      grid->states = Grid_InitStates(rows, cols);

      if (grid->ships && grid->states)
      {
         result = (Grid_ClearShips(grid->ships, rows, cols) &&
                   Grid_ClearStates(grid->states, rows, cols) &&
                   Grid_InitMeta(&grid->meta, rows, cols, 0, 1));
      }
   }
   return result;
}

ShipType_e * Grid_InitShips(uint rows, uint cols)
{
   ShipType_e * ships = NULL;
   if (rows > 0 && cols > 0)
   {
      ships = malloc(rows * cols * sizeof(ShipType_e));
   }
   return ships;
}

GridState_e * Grid_InitStates(uint rows, uint cols)
{
   GridState_e * states = NULL;
   if (rows > 0 && cols > 0)
   {
      states = malloc(rows * cols * sizeof(GridState_e));
   }
   return states;
}

bool Grid_ClearShips(
   ShipType_e * const ships,
   uint const rows,
   uint const cols)
{
   bool result = false;
   if (ships && rows > 0 && cols > 0)
   {
      memset(ships, 0, rows * cols * sizeof(ShipType_e));
      result = true;
   }
   return result;
}

bool Grid_ClearStates(
   GridState_e * const states,
   uint const rows,
   uint const cols)
{
   bool result = false;
   if (states && rows > 0 && cols > 0)
   {
      memset(states, 0, rows * cols * sizeof(GridState_e));
      result = true;
   }
   return result;
}

bool Grid_GetRowStr(
    Grid_t *const grid,
    GridState_e *const states,
    int const row,
    char *const line,
    size_t const line_size,
    size_t *const line_pos)
{
   GridMeta_t const * meta = NULL;
   char cell_str[SIZE_CELL_STR + 1] = {0};
   ShipType_e ship = SHIP_NONE;
   GridState_e state = GRID_STATE_BLANK;
   int i = 0;
   size_t len = 0;
   bool result = false;

   if (grid &&
       Grid_IsValidMeta(&grid->meta) &&
       line &&
       line_size > 0 &&
       line_pos &&
       *line_pos < line_size)
   {
      meta = &grid->meta;
      if (row == GRID_ROW_TITLE)
      {
         // title row
         result = Grid_AppendLine(
             line,
             line_size,
             line_pos,
             meta->row_width - 1,
             (states) ? STR_TITLE_OFFENSE : STR_TITLE_DEFENSE,
             SIZE_TITLE_STR);
         // row filler
         len = meta->side_len + 6 * SIZE_GRID_SPACE - meta->row_width - SIZE_TITLE_STR;
         RepeatChar(
             line + *line_pos,
             len,
             ' ');
         *line_pos += len - 1;
      }
      else if (row == GRID_ROW_HEADER)
      {
         // header row
         // grid corner
         result = Grid_AppendLine(
             line,
             line_size,
             line_pos,
             meta->row_width - 1,
             meta->corner_str,
             SIZE_GRID_SPACE);
         if (result)
         {
            // column labels
            for (uint col = 0; col < grid->cols; col++)
            {
               cell_str[0] = '\0';
               result = (Coord_ColToChar(
                             (int)col,
                             &cell_str[0]) &&
                         Grid_AppendLine(
                             line,
                             line_size,
                             line_pos,
                             SIZE_GRID_SPACE,
                             cell_str,
                             meta->col_width));
               if (!result)
               {
                  break;
               }
            }
         }
         if (result)
         {
            // grid corner
            result = Grid_AppendLine(
                line,
                line_size,
                line_pos,
                SIZE_GRID_SPACE,
                STR_GRID_CORNER,
                SIZE_GRID_SPACE);
         }
      }
      else if (row == (int)grid->rows)
      {
         // footer row
         // grid corner
         // row filler
         // grid corner
         result = Grid_AppendLine(
                      line,
                      line_size,
                      line_pos,
                      meta->row_width - 1,
                      meta->corner_str,
                      SIZE_GRID_SPACE) &&
                  Grid_AppendLine(
                      line,
                      line_size,
                      line_pos,
                      SIZE_GRID_SPACE,
                      meta->side_str,
                      meta->side_len - 1) &&
                  Grid_AppendLine(
                      line,
                      line_size,
                      line_pos,
                      SIZE_GRID_SPACE,
                      STR_GRID_CORNER,
                      SIZE_GRID_SPACE);
      }
      else
      {
         // numbered rows
         result = true;
         // row label
         if (snprintf(
                 line + *line_pos, line_size - *line_pos,
                 "%*u", (int)meta->row_width, row + 1) > 0)
         {
            *line_pos += (meta->row_width);
         }
         else
         {
            result = false;
         }
         if (result)
         {
            // cell states
            for (int col = 0; col < (int)grid->cols; col++)
            {
               i = row * (int)(grid->cols) + col;
               state = (states) ? states[i] : grid->states[i];
               Grid_StateToStr(state, cell_str, SIZE_CELL_STR + 1);
               if (!states && state == GRID_STATE_BLANK)
               {
                  ship = grid->ships[i];
                  Grid_ShipTypeToStr(ship, cell_str, SIZE_CELL_STR + 1);
               }
               result = Grid_AppendLine(
                   line,
                   line_size,
                   line_pos,
                   SIZE_GRID_SPACE,
                   cell_str,
                   meta->col_width);
               if (!result)
               {
                  break;
               }
            }
         }
         if (result)
         {
            // column filler
            result = Grid_AppendLine(
                line,
                line_size,
                line_pos,
                SIZE_GRID_SPACE,
                STR_GRID_SIDE_V,
                SIZE_GRID_SPACE);
         }
      }
   }

   return result;
}

bool Grid_InitMeta(GridMeta_t* meta,
   size_t row_size, size_t col_size,
   size_t row_width, size_t col_width)
{
   bool result = false;
   if (meta)
   {
      meta->row_width = (row_width)
         ? row_width : (size_t)CalcNumWidth((int)row_size);

      meta->col_width = (col_width)
         ? col_width : (size_t)CalcNumWidth((int)col_size);

      meta->corner_len = meta->col_width + SIZE_GRID_SPACE;
      char * corner_char = STR_GRID_CORNER;

      meta->side_len = (meta->col_width + SIZE_GRID_SPACE) * col_size;
      char * side_char = STR_GRID_SIDE_H;

      meta->corner_str = malloc(meta->corner_len);
      meta->side_str = malloc(meta->side_len);

      result = (meta->corner_str &&
                meta->side_str &&
                RepeatChar(
                   meta->corner_str,
                   meta->corner_len,
                   corner_char[0]) &&
                RepeatChar(
                   meta->side_str,
                   meta->side_len,
                   side_char[0]));
   }
   return result;
}

bool Grid_IsValidMeta(GridMeta_t const * const meta)
{
   bool result = false;
   if (meta)
   {
      result = (
         meta->row_width > 0 &&
         meta->col_width > 0 &&
         meta->corner_len > 0 &&
         meta->side_len > 0 &&
         meta->corner_str &&
         meta->side_str);
   }
   return result;
}

GridStatus_e Grid_PlaceShip(const Grid_t *grid, const Ship_t *ship)
{
   Ship_Info_t const * ship_info = NULL;
   GridStatus_e result = GRID_STATUS_NULL;
   Coord_t point = { 0 };
   if (grid && grid->ships && ship)
   {
      ship_info = Ship_GetInfo(ship->type);
      result = Grid_CheckShip(grid, ship);
      if (ship_info && (GRID_STATUS_OK == result))
      {
         for (uint i = 0; i < ship_info->length; i++)
         {
            point = Ship_GetPoint(ship, i);
            grid->ships[point.row * (int)grid->cols +
                          point.col] = ship->type;
         }
      }
   }
   return result;
}

GridStatus_e Grid_CheckShip(const Grid_t *grid, const Ship_t *ship)
{
   GridStatus_e result = GRID_STATUS_NULL;
   if (grid && grid->ships && ship)
   {
      const Ship_Info_t *ship_info = Ship_GetInfo(ship->type);
      if (ship_info)
      {
         result = GRID_STATUS_OK;
         for (uint i = 0; i < ship_info->length; i++)
         {
            Coord_t point = Ship_GetPoint(ship, i);
            if (!(point.row >= 0 && point.row < (int)grid->rows &&
                  point.col >= 0 && point.col < (int)grid->cols))
            {
               result |= GRID_STATUS_BORDER;
            }
            else
            {
               ShipType_e ship_type =
                   grid->ships[point.row * (int)grid->cols +
                                 point.col];
               if (ship_type != SHIP_NONE)
               {
                  result |= GRID_STATUS_COLLISION;
               }
            }
         }
         debug_print("(n,r,c,h,s)=(%c,%d,%d,%c,%c%c)\n",
                     ship_info->name[0],
                     ship->location.row,
                     ship->location.col,
                     Heading_GetChar(ship->heading),
                     (result & GRID_STATUS_COLLISION ? '1' : '0'),
                     (result & GRID_STATUS_BORDER) ? '1' : '0');
      }
   }
   return result;
}

bool Grid_ShipTypeToStr(
   ShipType_e const type,
   char * const str,
   size_t const str_len)
{
   Ship_Info_t const * ship_info = NULL;
   char const * state_str = NULL;
   bool result = false;
   if (str && str_len > 0)
   {
      if (SHIP_NONE == type)
      {
         state_str = Grid_GetStateStr(GRID_STATE_BLANK);
         if (state_str &&
            (snprintf(str, str_len, "%s", state_str) > 0))
         {
            result = true;
         }
      }
      else
      {
         ship_info = Ship_GetInfo(type);
         if (ship_info &&
             (snprintf(str, str_len, "%c", ship_info->name[0]) > 0))
         {
            result = true;
         }
      }
   }
   return result;
}

bool Grid_StateToStr(
   GridState_e const state,
   char * const str,
   size_t const str_len)
{
   bool result = false;
   char const * state_str = NULL;
   if (str && str_len > 0)
   {
      state_str = Grid_GetStateStr(state);
      if (state_str)
      {
         result = (snprintf(str, str_len, "%s", state_str) > 0);
      }
   }
   return result;
}

bool Grid_AppendLine(
    char *const line,
    size_t const line_size,
    size_t *const line_pos,
    size_t const space_len,
    char *const str,
    size_t const str_size)
{
   bool result = false;
   if (line &&
      line_size > 0 &&
      line_pos &&
      *line_pos < line_size)
   {
      if (snprintf(
              line + *line_pos,
              line_size - *line_pos,
              "%*s%*s",
              (int)space_len,
              "",
              (int)str_size,
              str) > 0)
      {
         *line_pos += (space_len + str_size);
         result = true;
      }
   }
   return result;
}
