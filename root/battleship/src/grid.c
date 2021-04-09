/*
 * grid.c
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#include "battleship/ui.h"
#include "battleship/util.h"
#include "battleship/grid.h"

static Grid_Status_t Grid_Check_Ship(const Grid_t *grid, const Ship_t *ship);

Status_t Grid_Init(Grid_t *grid, uint rows, uint cols)
{
   Status_t status = STATUS_OK;
   if (!grid)
   {
      return STATUS_ERROR;
   }
   grid->rows = rows;
   grid->cols = cols;
   status |= Grid_Init_Defense(grid);
   status |= Grid_Init_Offense(grid);
   return status;
}

Status_t Grid_Init_Defense(Grid_t *grid)
{
   if (!grid)
   {
      return STATUS_ERROR;
   }
   grid->defense = malloc(grid->rows * grid->cols * sizeof(Grid_State_t));
   if (!grid->defense)
   {
      return STATUS_ERROR;
   }
   Grid_Clear_Defense(grid);
   return STATUS_OK;
}

Status_t Grid_Init_Offense(Grid_t *grid)
{
   if (!grid)
   {
      return STATUS_ERROR;
   }
   grid->offense = malloc(grid->rows * grid->cols * sizeof(Hit_State_t));
   if (!grid->offense)
   {
      return STATUS_ERROR;
   }
   Grid_Clear_Offense(grid);
   return STATUS_OK;
}

void Grid_Clear_Defense(const Grid_t *grid)
{
   if (grid && grid->defense)
   {
      memset(grid->defense, 0, grid->rows * grid->cols * sizeof(Grid_State_t));
   }
}

void Grid_Clear_Offense(const Grid_t *grid)
{
   if (grid && grid->offense)
   {
      memset(grid->offense, 0, grid->rows * grid->cols * sizeof(Hit_State_t));
   }
}

void Grid_Meta_Init(Grid_Meta_t* meta,
		size_t row_size, size_t col_size,
		size_t row_width, size_t col_width)
{
   if (meta)
   {
      meta->row_width = (row_width)
         ? row_width : (size_t)CalcNumWidth((int)row_size);

      meta->col_width = (col_width)
         ? col_width : (size_t)CalcNumWidth((int)col_size);

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
   if (!grid || !ship)
   {
      return GRID_STATUS_NULL;
   }
   const Ship_Info_t *ship_info = Ship_Get_Info(ship->type);
   if (!ship_info)
   {
      return GRID_STATUS_NULL;
   }
   Grid_Status_t status = Grid_Check_Ship(grid, ship);
   if (GRID_STATUS_OK != status)
   {
      return status;
   }
   for (uint i = 0; i < ship_info->length; i++)
   {
      Coord_t point = Ship_Get_Point(ship, i);
      grid->defense[point.row * (int) grid->cols + point.col].ship_type = ship->type;
   }
   return status;
}

static Grid_Status_t Grid_Check_Ship(const Grid_t *grid, const Ship_t *ship)
{
   Grid_Status_t status = GRID_STATUS_OK;
   const Ship_Info_t *ship_info = Ship_Get_Info(ship->type);
   if (!grid || !ship || !ship_info)
   {
      return GRID_STATUS_NULL;
   }
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
         Ship_Type_t ship_type =
            grid->defense[
               point.row * (int) grid->cols +
               point.col].ship_type;
         if (ship_type != SHIP_NONE)
         {
            status |= GRID_STATUS_COLLISION;
         }
      }
   }
   debug_print("(n,r,c,h,s)=(%c,%d,%d,%s,%c%c)\n",
         ship_info->name[0],
         ship->location.row,
         ship->location.col,
         Heading_Get_Str(ship->heading),
         (status & GRID_STATUS_COLLISION ? '1' : '0'),
         (status & GRID_STATUS_BORDER) ? '1' : '0');
   return status;
}
