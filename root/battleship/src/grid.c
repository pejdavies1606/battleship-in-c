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

const char * Grid_Get_State_Str(Hit_State_e const state)
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

static Grid_Status_e Grid_Check_Ship(
   const Grid_t *grid,
   const Ship_t *ship);

bool Grid_Init(Grid_t *grid, uint rows, uint cols)
{
   bool result = false;
   if (grid)
   {
      grid->rows = rows;
      grid->cols = cols;
      result = ((Grid_Init_Defense(grid)) &&
                (Grid_Init_Offense(grid)) &&
                (Grid_Meta_Init(&grid->meta, rows, cols, 0, 1)));
   }
   return result;
}

bool Grid_Init_Defense(Grid_t *grid)
{
   bool result = false;
   if (grid)
   {
      grid->defense = malloc(grid->rows * grid->cols * sizeof(Grid_State_t));
      if (grid->defense)
      {
         result = Grid_Clear_Defense(grid);
      }
   }
   return result;
}

bool Grid_Init_Offense(Grid_t *grid)
{
   bool result = false;
   if (grid)
   {
      grid->offense = malloc(grid->rows * grid->cols * sizeof(Hit_State_e));
      if (grid->offense)
      {
         result = Grid_Clear_Offense(grid);
      }
   }
   return result;
}

bool Grid_Clear_Defense(const Grid_t *grid)
{
   bool result = false;
   if (grid && grid->defense)
   {
      memset(grid->defense, 0, grid->rows * grid->cols * sizeof(Grid_State_t));
      result = true;
   }
   return result;
}

bool Grid_Clear_Offense(const Grid_t *grid)
{
   bool result = false;
   if (grid && grid->offense)
   {
      memset(grid->offense, 0, grid->rows * grid->cols * sizeof(Hit_State_e));
      result = true;
   }
   return result;
}

bool Grid_Get_Row_Defense(
   Grid_t const * const grid,
   int row,
   char * const line,
   size_t line_size)
{
   Grid_State_t * defense = NULL;
   Grid_Meta_t const * meta = NULL;
   char ship_str[SIZE_STATE_STR + 1] = { 0 };
   char state_str[SIZE_STATE_STR + 1] = { 0 };
   char col_char = '\0';
   Grid_State_t cell = { 0 };
   size_t line_pos = 0;
   bool result = false;

   if (NULL != grid &&
       NULL != grid->defense &&
       Grid_Meta_Is_Valid(&grid->meta) &&
       NULL != line &&
       line_size > 0)
   {
      defense = grid->defense;
      meta = &grid->meta;
      if (row == GRID_ROW_HEADER)
      {
         // header row
         result = true;
         // grid corner
         if (snprintf(
                 line + line_pos, line_size - line_pos,
                 "%*s%.*s", (int)(meta->row_width - 1), "",
                 SIZE_GRID_SPACE, meta->corner_str) > 0)
         {
            line_pos += (meta->row_width - 1 + SIZE_GRID_SPACE);
         }
         else
         {
            result = false;
         }
         if (result)
         {
            // column labels
            for (uint col = 0; col < grid->cols; col++)
            {
               col_char = '\0';
               if (Coord_ColToChar((int)col, &col_char) &&
                   (snprintf(
                             line + line_pos, line_size - line_pos,
                             "%*s%-*c", SIZE_GRID_SPACE, "",
                             (int)meta->col_width, col_char) > 0))
               {
                  line_pos += (SIZE_GRID_SPACE + meta->col_width);
               }
               else
               {
                  result = false;
                  break;
               }
            }
         }
         if (result)
         {
            // grid corner
            if (snprintf(
                         line + line_pos, line_size - line_pos,
                         "%*s%s", SIZE_GRID_SPACE, "",
                         STR_GRID_CORNER) > 0)
            {
               line_pos += (SIZE_GRID_SPACE + 1);
            }
            else
            {
               result = false;
            }
         }
      }
      else if (row == (int) grid->rows)
      {
         // footer row
         result = true;
         // grid corner
         if (snprintf(
                      line + line_pos, line_size - line_pos,
                      "%*s%.*s", (int)(meta->row_width - 1), "",
                      SIZE_GRID_SPACE, meta->corner_str) > 0)
         {
            line_pos += (meta->row_width - 1 + SIZE_GRID_SPACE);
         }
         else
         {
            result = false;
         }
         if (result)
         {
            // row filler
            if (snprintf(
                         line + line_pos, line_size - line_pos,
                         "%*s%.*s", SIZE_GRID_SPACE, "",
                         (int)meta->side_len, meta->side_str) > 0)
            {
               line_pos += (SIZE_GRID_SPACE + meta->side_len - 1);
            }
            else
            {
               result = false;
            }
         }
         if (result)
         {
            // grid corner
            if (snprintf(
                         line + line_pos, line_size - line_pos,
                         "%*s%s", SIZE_GRID_SPACE, "",
                         STR_GRID_CORNER) > 0)
            {
               line_pos += (SIZE_GRID_SPACE + 1);
            }
            else
            {
               result = false;
            }
         }
      }
      else
      {
         // numbered rows
         result = true;
         // row label
         if (snprintf(
                      line + line_pos, line_size - line_pos,
                      "%*u", (int)meta->row_width, row + 1) > 0)
         {
            line_pos += (meta->row_width);
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
               cell = defense[row * (int)(grid->cols) + col];
               if (Ship_Type_To_Str(cell.ship_type, ship_str, SIZE_STATE_STR + 1) &&
                   Hit_State_To_Str(cell.hit_state, state_str, SIZE_STATE_STR + 1) &&
                  (snprintf(
                      line + line_pos, line_size - line_pos,
                      "%*s%*s",
                      SIZE_GRID_SPACE, "",
                      (int)meta->col_width,
                      (cell.hit_state == HIT_STATE_HIT) ? state_str : ship_str) > 0))
               {
                  line_pos += (SIZE_GRID_SPACE + meta->col_width);
               }
               else
               {
                  result = false;
                  break;
               }
            }
         }
         if (result)
         {
            // column filler
            if (snprintf(
                         line + line_pos, line_size - line_pos,
                         "%*s%s", SIZE_GRID_SPACE, "", STR_GRID_SIDE_V) > 0)
            {
               line_pos += (SIZE_GRID_SPACE + 1);
            }
            else
            {
               result = false;
            }
         }
      }
   }

   return result;
}

bool Grid_Meta_Init(Grid_Meta_t* meta,
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

bool Grid_Meta_Is_Valid(Grid_Meta_t const * const meta)
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

Grid_Status_e Grid_Place_Ship(const Grid_t *grid, const Ship_t *ship)
{
   Ship_Info_t const * ship_info = NULL;
   Grid_Status_e result = GRID_STATUS_NULL;
   Coord_t point = { 0 };
   if (grid && grid->defense && ship)
   {
      ship_info = Ship_Get_Info(ship->type);
      result = Grid_Check_Ship(grid, ship);
      if (ship_info && (GRID_STATUS_OK == result))
      {
         for (uint i = 0; i < ship_info->length; i++)
         {
            point = Ship_Get_Point(ship, i);
            grid->defense[point.row * (int)grid->cols +
                          point.col]
                .ship_type = ship->type;
         }
      }
   }
   return result;
}

Grid_Status_e Grid_Check_Ship(const Grid_t *grid, const Ship_t *ship)
{
   Grid_Status_e result = GRID_STATUS_NULL;
   if (grid && grid->defense && ship)
   {
      const Ship_Info_t *ship_info = Ship_Get_Info(ship->type);
      if (ship_info)
      {
         result = GRID_STATUS_OK;
         for (uint i = 0; i < ship_info->length; i++)
         {
            Coord_t point = Ship_Get_Point(ship, i);
            if (!(point.row >= 0 && point.row < (int)grid->rows &&
                  point.col >= 0 && point.col < (int)grid->cols))
            {
               result |= GRID_STATUS_BORDER;
            }
            else
            {
               Ship_Type_e ship_type =
                   grid->defense[point.row * (int)grid->cols +
                                 point.col]
                       .ship_type;
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
                     Heading_Get_Char(ship->heading),
                     (result & GRID_STATUS_COLLISION ? '1' : '0'),
                     (result & GRID_STATUS_BORDER) ? '1' : '0');
      }
   }
   return result;
}

bool Ship_Type_To_Str(
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
         state_str = Grid_Get_State_Str(HIT_STATE_BLANK);
         if (state_str &&
            (snprintf(str, str_len, "%s", state_str) > 0))
         {
            result = true;
         }
      }
      else
      {
         ship_info = Ship_Get_Info(type);
         if (ship_info &&
             (snprintf(str, str_len, "%c", ship_info->name[0]) > 0))
         {
            result = true;
         }
      }
   }
   return result;
}

bool Hit_State_To_Str(
   Hit_State_e const state,
   char * const str,
   size_t const str_len)
{
   bool result = false;
   char const * state_str = NULL;
   if (str && str_len > 0)
   {
      state_str = Grid_Get_State_Str(state);
      result = (state_str &&
                (snprintf(str, str_len, "%s", state_str) > 0));
   }
   return result;
}
