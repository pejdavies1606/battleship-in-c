/*
 * grid.c
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#include "battleship/ui.h"
#include "battleship/util.h"
#include "battleship/grid.h"

typedef struct HitStateInfo
{
   Hit_State_e state;
   char const * str;
} HitStateInfo_t;

#define HIT_STATE_COUNT 3
static HitStateInfo_t const HIT_STATE_TABLE[HIT_STATE_COUNT] =
{
   { HIT_STATE_BLANK, " " },
   { HIT_STATE_HIT,   "x" },
   { HIT_STATE_MISS,  "o" },
};

const char * Grid_GetHitStateStr(Hit_State_e const state)
{
   char const * str = NULL;
   for (uint i = 0; i < HIT_STATE_COUNT; i++)
   {
      if (state == HIT_STATE_TABLE[i].state)
      {
         str = HIT_STATE_TABLE[i].str;
      }
   }
   return str;
}

static Grid_Status_e Grid_CheckShip(
   const Grid_t *grid,
   const Ship_t *ship);

static bool Grid_AppendLine(
    char *const line,
    size_t const line_size,
    size_t *const line_pos,
    size_t const space_len,
    char *const str,
    size_t const str_size);

bool Grid_Init(Grid_t *grid, uint rows, uint cols)
{
   bool result = false;
   if (grid)
   {
      grid->rows = rows;
      grid->cols = cols;
      result = ((Grid_InitShips(grid)) &&
                (Grid_InitHits(grid)) &&
                (Grid_InitMeta(&grid->meta, rows, cols, 0, 1)));
   }
   return result;
}

bool Grid_InitShips(Grid_t *grid)
{
   bool result = false;
   if (grid && grid->rows > 0 && grid->cols > 0)
   {
      grid->ships = malloc(grid->rows * grid->cols * sizeof(Ship_Type_e));
      if (grid->ships)
      {
         result = Grid_ClearShips(grid);
      }
   }
   return result;
}

bool Grid_InitHits(Grid_t *grid)
{
   bool result = false;
   if (grid && grid->rows > 0 && grid->cols > 0)
   {
      grid->hits = malloc(grid->rows * grid->cols * sizeof(Hit_State_e));
      if (grid->hits)
      {
         result = Grid_ClearHits(grid);
      }
   }
   return result;
}

bool Grid_ClearShips(const Grid_t *grid)
{
   bool result = false;
   if (grid && grid->ships)
   {
      memset(grid->ships, 0, grid->rows * grid->cols * sizeof(Ship_Type_e));
      result = true;
   }
   return result;
}

bool Grid_ClearHits(const Grid_t *grid)
{
   bool result = false;
   if (grid && grid->hits)
   {
      memset(grid->hits, 0, grid->rows * grid->cols * sizeof(Hit_State_e));
      result = true;
   }
   return result;
}

bool Grid_GetRowStr(
    Grid_t const *const grid,
    bool showOffense,
    int row,
    char *const line,
    size_t line_size,
    size_t *line_pos)
{
   Ship_Type_e * ships = NULL;
   Hit_State_e * hits = NULL;
   Grid_Meta_t const * meta = NULL;
   char ship_str[SIZE_STATE_STR + 1] = {0};
   char state_str[SIZE_STATE_STR + 1] = {0};
   char col_str[2] = {0};
   Ship_Type_e ship = SHIP_NONE;
   Hit_State_e hit = HIT_STATE_BLANK;
   int i = 0;
   size_t len = 0;
   bool result = false;

   if (NULL != grid &&
       Grid_IsValidMeta(&grid->meta) &&
       NULL != line &&
       line_size > 0 &&
       line_pos &&
       *line_pos < line_size)
   {
      if (showOffense)
      {
         // TODO get hits array of other player
         hits = grid->hits; // show own hits for now
      }
      else
      {
         hits = grid->hits;
         ships = grid->ships;
      }

      if (ships || hits)
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
                (showOffense) ? STR_TITLE_OFFENSE : STR_TITLE_DEFENSE,
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
                  col_str[0] = '\0';
                  result = (Coord_ColToChar(
                                (int)col,
                                &col_str[0]) &&
                            Grid_AppendLine(
                                line,
                                line_size,
                                line_pos,
                                SIZE_GRID_SPACE,
                                col_str,
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
                  hits[i] = (i % 2) ? HIT_STATE_HIT : HIT_STATE_MISS;
                  if (showOffense)
                  {
                     hit = hits[i];
                     result =
                         Grid_HitStateToStr(
                            hit,
                            state_str,
                            SIZE_STATE_STR + 1) &&
                         Grid_AppendLine(
                             line,
                             line_size,
                             line_pos,
                             SIZE_GRID_SPACE,
                             state_str,
                             meta->col_width);
                     if (!result)
                     {
                        break;
                     }
                  }
                  else
                  {
                     hit = hits[i];
                     ship = ships[i];
                     result =
                         Grid_HitStateToStr(
                             hit,
                             state_str,
                             SIZE_STATE_STR + 1) &&
                         Ship_TypeToStr(
                             ship,
                             ship_str,
                             SIZE_STATE_STR + 1) &&
                         Grid_AppendLine(
                             line,
                             line_size,
                             line_pos,
                             SIZE_GRID_SPACE,
                             (hit == HIT_STATE_BLANK) ? ship_str : state_str ,
                             meta->col_width);
                     if (!result)
                     {
                        break;
                     }
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
   }

   return result;
}

bool Grid_InitMeta(Grid_Meta_t* meta,
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
      String_t corner_char = STR_GRID_CORNER;

      meta->side_len = (meta->col_width + SIZE_GRID_SPACE) * col_size;
      String_t side_char = STR_GRID_SIDE_H;

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

bool Grid_IsValidMeta(Grid_Meta_t const * const meta)
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

Grid_Status_e Grid_PlaceShip(const Grid_t *grid, const Ship_t *ship)
{
   Ship_Info_t const * ship_info = NULL;
   Grid_Status_e result = GRID_STATUS_NULL;
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

Grid_Status_e Grid_CheckShip(const Grid_t *grid, const Ship_t *ship)
{
   Grid_Status_e result = GRID_STATUS_NULL;
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
               Ship_Type_e ship_type =
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

bool Ship_TypeToStr(
   Ship_Type_e const type,
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
         state_str = Grid_GetHitStateStr(HIT_STATE_BLANK);
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

bool Grid_HitStateToStr(
   Hit_State_e const state,
   char * const str,
   size_t const str_len)
{
   bool result = false;
   char const * state_str = NULL;
   if (str && str_len > 0)
   {
      state_str = Grid_GetHitStateStr(state);
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