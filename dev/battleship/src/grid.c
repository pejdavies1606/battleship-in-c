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

char const * Grid_GetStateStr(GridState_e const state)
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

static bool _InitMeta(
   GridMeta_t * const meta,
   size_t const row_size,
   size_t const col_size,
   size_t const row_width,
   size_t const col_width);

static void _DestroyMeta(
   GridMeta_t * const meta);

static bool _IsValidMeta(
   GridMeta_t const * const meta);

static bool _CheckShip(
   const Grid_t * const grid,
   const Ship_t * const ship,
   GridStatus_e * const status);

static bool _SetShip(
   ShipType_e * const ships,
   const Ship_t * const ship,
   uint len,
   ShipType_e type);

static bool _AppendLine(
   char * const line,
   size_t const line_size,
   size_t * const line_pos,
   size_t const space_len,
   char * const str,
   size_t const str_size);

bool Grid_Init(
   Grid_t * const grid)
{
   bool result = false;
   if (grid)
   {
      grid->rows = MAX_COORD_ROW;
      grid->cols = MAX_COORD_COL;
      memset(grid->ships, 0, SIZE_GRID * sizeof(ShipType_e));
      memset(grid->states, 0, SIZE_GRID * sizeof(GridState_e));
      result = _InitMeta(&grid->meta, grid->rows, grid->cols, 0, 1);
   }
   return result;
}

void Grid_Destroy(
   Grid_t * const grid)
{
   if (grid)
   {
      _DestroyMeta(&grid->meta);
   }
}

bool Grid_Clear(
   Grid_t * const grid)
{
   bool result = false;
   if (grid)
   {
      memset(grid->ships, 0, SIZE_GRID * sizeof(ShipType_e));
      memset(grid->states, 0, SIZE_GRID * sizeof(GridState_e));
      result = true;
   }
   return result;
}

bool Grid_GetRowStr(
    Grid_t const *const grid,
    GridState_e const *const states,
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
       _IsValidMeta(&grid->meta) &&
       line &&
       line_size > 0 &&
       line_pos &&
       *line_pos < line_size)
   {
      meta = &grid->meta;
      if (row == GRID_ROW_TITLE)
      {
         // title row
         result = _AppendLine(
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
         result = _AppendLine(
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
                         _AppendLine(
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
            result = _AppendLine(
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
         result = _AppendLine(
                      line,
                      line_size,
                      line_pos,
                      meta->row_width - 1,
                      meta->corner_str,
                      SIZE_GRID_SPACE) &&
                  _AppendLine(
                      line,
                      line_size,
                      line_pos,
                      SIZE_GRID_SPACE,
                      meta->side_str,
                      meta->side_len - 1) &&
                  _AppendLine(
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
               result = _AppendLine(
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
            result = _AppendLine(
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

bool Grid_PlaceShip(
   Grid_t * const grid,
   Ship_t * const player_ship,
   const Ship_t * const ship,
   GridStatus_e * const status)
{
   bool result = false;
   if (grid && ship && status)
   {
      result = _CheckShip(grid, ship, status);
      if (GRID_STATUS_OK == *status)
      {
         Ship_Info_t const *ship_info = Ship_GetInfo(ship->type);
         if (ship_info)
         {
            // Check if ship has been placed before,
            // otherwise location defaults to 0,0 (A1)
            // and may clear a ship already placed there
            if (player_ship->heading != HEADING_UNKNOWN)
            {
               // Remove previous location of ship on grid
               result = _SetShip(
                   grid->ships,
                   player_ship,
                   ship_info->length,
                   SHIP_NONE);
            }
            // Place new location of ship on grid
            result = _SetShip(
                grid->ships,
                ship,
                ship_info->length,
                ship->type);
            // Update player ship location and heading
            player_ship->location = ship->location;
            player_ship->heading = ship->heading;
            result = true;
         }
      }
   }
   return result;
}

bool Grid_PlaceHit(
   const Grid_t * const grid,
   GridState_e * const states,
   const Coord_t * const location,
   bool * const hit)
{
   bool result = false;
   if (grid && states && location && hit)
   {
      int i = location->row * (int)grid->cols + location->col;
      ShipType_e ship_type = grid->ships[i];
      *hit = (ship_type != SHIP_NONE);
      states[i] = (*hit) ? GRID_STATE_HIT : GRID_STATE_MISS;
      result = true;
   }
   return result;
}

bool Grid_ShipTypeToStr(
   ShipType_e const type,
   char const * const str,
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
            (snprintf((char *)str, str_len, "%s", state_str) > 0))
         {
            result = true;
         }
      }
      else
      {
         ship_info = Ship_GetInfo(type);
         if (ship_info &&
             (snprintf((char *)str, str_len, "%c", ship_info->name[0]) > 0))
         {
            result = true;
         }
      }
   }
   return result;
}

bool Grid_StateToStr(
   GridState_e const state,
   char const * const str,
   size_t const str_len)
{
   bool result = false;
   char const * state_str = NULL;
   if (str && str_len > 0)
   {
      state_str = Grid_GetStateStr(state);
      if (state_str)
      {
         result = (snprintf((char *)str, str_len, "%s", state_str) > 0);
      }
   }
   return result;
}

bool _InitMeta(
   GridMeta_t * const meta,
   size_t const row_size,
   size_t const col_size,
   size_t const row_width,
   size_t const col_width)
{
   bool result = false;
   if (meta)
   {
      meta->row_width = (row_width)
         ? row_width : (uint)CalcNumWidth((int)row_size);

      meta->col_width = (col_width)
         ? col_width : (uint)CalcNumWidth((int)col_size);

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

void _DestroyMeta(GridMeta_t * const meta)
{
   if (meta)
   {
      if (meta->corner_str)
      {
         free(meta->corner_str);
         meta->corner_str = NULL;
      }
      if (meta->side_str)
      {
         free(meta->side_str);
         meta->side_str = NULL;
      }
   }
}

bool _IsValidMeta(GridMeta_t const * const meta)
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

bool _CheckShip(
   const Grid_t * const grid,
   const Ship_t * const ship,
   GridStatus_e * const status)
{
   bool result = false;
   if (grid && ship && status)
   {
      const Ship_Info_t *ship_info = Ship_GetInfo(ship->type);
      if (ship_info)
      {
         *status = GRID_STATUS_OK;
         for (uint i = 0; i < ship_info->length; i++)
         {
            Coord_t loc = Ship_GetPoint(ship, i);
            if (!(loc.row >= 0 && loc.row < (int)grid->rows &&
                  loc.col >= 0 && loc.col < (int)grid->cols))
            {
               *status |= GRID_STATUS_BORDER;
            }
            else
            {
               ShipType_e ship_type =
                   grid->ships[loc.row * (int)grid->cols + loc.col];
               // Allow collision with same type to enable replacing
               if (ship_type != SHIP_NONE && ship_type != ship->type)
               {
                  *status |= GRID_STATUS_COLLISION;
               }
            }
         }
         result = true;
         debug_print("(n,r,c,h,s)=(%c,%d,%d,%c,%c%c)\n",
                     ship_info->name[0],
                     ship->location.row,
                     ship->location.col,
                     Heading_GetChar(ship->heading),
                     (*status & GRID_STATUS_COLLISION ? '1' : '0'),
                     (*status & GRID_STATUS_BORDER) ? '1' : '0');
      }
   }
   return result;
}

bool _SetShip(
   ShipType_e * const ships,
   const Ship_t * const ship,
   uint len,
   ShipType_e type)
{
   bool result = false;
   if (ships && ship)
   {
      for (uint i = 0; i < len; i++)
      {
         Coord_t loc = Ship_GetPoint(ship, i);
         if (loc.col < MAX_COORD_COL && loc.row < MAX_COORD_ROW)
         {
            ships[loc.row * MAX_COORD_COL + loc.col] = type;
         }
      }
      result = true;
   }
   return result;
}

bool _AppendLine(
    char * const line,
    size_t const line_size,
    size_t * const line_pos,
    size_t const space_len,
    char * const str,
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
