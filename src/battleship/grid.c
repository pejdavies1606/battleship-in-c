/*
 * grid.c
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#include "ui.h"
#include "util.h"
#include "grid.h"

static Grid_Status_t Grid_Check_Ship(const Grid_t *grid, const Ship_t *ship);

Grid_State_t * Grid_Init_Defense(size_t row_size, size_t col_size)
{
   Grid_State_t *defense = malloc(row_size * col_size * sizeof(Grid_State_t));
   memset(defense, 0, row_size * col_size * sizeof(Grid_State_t));
   return defense;
}

Hit_State_t * Grid_Init_Offense(size_t row_size, size_t col_size)
{
   Hit_State_t *offense = malloc(row_size * col_size * sizeof(Hit_State_t));
   memset(offense, 0, row_size * col_size * sizeof(Hit_State_t));
   return offense;
}

void Grid_Meta_Init(Grid_Meta_t* meta, size_t col_size)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   if (meta)
   {
      meta->col_width = (size_t)CalcNumWidth((int)col_size);

      meta->corner_len = meta->col_width + SIZE_GRID_SPACE;
      String_t corner_char = STR_GRID_CORNER;
      meta->corner_str = malloc(meta->corner_len);
      RepeatChar(meta->corner_str, meta->corner_len, corner_char[0]);

      meta->side_len = (meta->col_width + SIZE_GRID_SPACE) * col_size;
      String_t side_char = STR_GRID_SIDE_H;
      meta->side_str = malloc(meta->side_len);
      RepeatChar(meta->side_str, meta->side_len, side_char[0]);
   }
}

Grid_Status_t Grid_Place_Ship(const Grid_t *grid, const Ship_t *ship)
{
   const Ship_Info_t *ship_info = Ship_Get_Info(ship->type);
   Grid_Status_t status = Grid_Check_Ship(grid, ship);
   if (GRID_STATUS_OK == status && ship_info)
   {
      for (uint i = 0; i < ship_info->length; i++)
      {
         Coord_t point = Ship_Get_Point(ship, i);
         grid->defense[point.row * (int) grid->cols + point.col].ship_type = ship->type;
      }
   }
   else
   {
      status = GRID_STATUS_UNKNOWN;
   }
   return status;
}

static Grid_Status_t Grid_Check_Ship(const Grid_t *grid, const Ship_t *ship)
{
   Grid_Status_t status = GRID_STATUS_OK;
   const Ship_Info_t *ship_info = Ship_Get_Info(ship->type);
   if (grid && ship && ship_info)
   {
      for (uint i = 0; i < ship_info->length; i++)
      {
         Coord_t point = Ship_Get_Point(ship, i);
         if (!(point.row >= 0 && point.row < (int) grid->rows &&
               point.col >= 0 && point.col < (int) grid->cols))
         {
            status |= GRID_STATUS_BORDER;
         }
         else
         {
            Ship_Type_t ship_type = grid->defense[point.row * (int) grid->cols + point.col].ship_type;
            if (ship_type != SHIP_NONE)
            {
               status = GRID_STATUS_COLLISION;
            }
         }
      }
   }
   else
   {
      status |= GRID_STATUS_UNKNOWN;
   }
   return status;
}
