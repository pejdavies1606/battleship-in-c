/*
 * grid.c
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#include "battleship/ui.h"
#include "battleship/util.h"
#include "battleship/grid.h"

static Grid_Status_e Grid_Check_Ship(const Grid_t *grid, const Ship_t *ship);

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

Grid_Status_e Grid_Place_Ship(const Grid_t *grid, const Ship_t *ship)
{
   Grid_Status_e result = GRID_STATUS_NULL;
   if (grid && grid->defense && ship)
   {
      const Ship_Info_t *ship_info = Ship_Get_Info(ship->type);
      if (ship_info)
      {
         result = Grid_Check_Ship(grid, ship);
         if (GRID_STATUS_OK == result)
         {
            for (uint i = 0; i < ship_info->length; i++)
            {
               Coord_t point = Ship_Get_Point(ship, i);
               grid->defense[point.row * (int)grid->cols +
                             point.col]
                   .ship_type = ship->type;
            }
         }
      }
   }
   return result;
}

static Grid_Status_e Grid_Check_Ship(const Grid_t *grid, const Ship_t *ship)
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

bool ShipTypeToStr(const Ship_Type_e type, char *str, const size_t str_len)
{
   bool result = false;
   if (str && str_len > 0)
   {
      if (SHIP_NONE == type)
      {
         snprintf(str, str_len, "%s", STR_STATE_BLANK);
      }
      else
      {
         const Ship_Info_t *ship_info = Ship_Get_Info(type);
         if (ship_info)
         {
            snprintf(str, str_len, "%c", ship_info->name[0]);
         }
      }
   }
   return result;
}

bool HitStateToStr(const Hit_State_e state, char *str, size_t str_len)
{
   bool result = false;
   if (str && str_len > 0)
   {
      switch (state)
      {
      case STATE_BLANK:
         snprintf(str, str_len, "%s", STR_STATE_BLANK);
         break;
      case STATE_HIT:
         snprintf(str, str_len, "%s", STR_STATE_HIT);
         break;
      case STATE_MISS:
         snprintf(str, str_len, "%s", STR_STATE_MISS);
         break;
      }
   }
   return result;
}
