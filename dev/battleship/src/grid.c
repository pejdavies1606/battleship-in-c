/*
 * grid.c
 *
 *  Created on: 26 Aug 2019
 *      Author: pejdavies1606
 */

#include "battleship/util.h"
#include "battleship/grid.h"

typedef struct GridStateInfo
{
   GridState_e state;
   char const *str;
} GridStateInfo_t;

#define GRID_STATE_COUNT 4
static GridStateInfo_t const GRID_STATE_TABLE[GRID_STATE_COUNT] =
{
   { GRID_STATE_BLANK, " " },
   { GRID_STATE_MISS,  "o" },
   { GRID_STATE_HIT,   "x" },
   { GRID_STATE_SUNK,  "X" },
};

char const *Grid_GetStateStr(GridState_e const state)
{
   char const *str = NULL;
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
   GridMeta_t *const meta);

static bool _IsValidMeta(
   GridMeta_t const *const meta);

static bool _CheckShip(
   Grid_t const *const grid,
   Ship_t const *const ship,
   GridStatus_e *const status);

static bool _CheckBorder(
   Grid_t const *const grid,
   Coord_t const *const loc,
   bool *const valid);

static bool _SetShip(
   Grid_t const *const grid,
   ShipType_e *const ships,
   Ship_t const *const ship,
   ShipType_e const type);

static bool _SetState(
    Grid_t const *const grid,
    GridState_e *const states,
    Ship_t const *const ship,
    GridState_e const state);

static bool _AppendRowTitle(
    Grid_t const *const grid,
    Line_t *const line,
    bool const off_grid);

static bool _AppendRowHeader(
    Grid_t const *const grid,
    Line_t *const line);

static bool _AppendRowFooter(
    Grid_t const *const grid,
    Line_t *const line);

static bool _AppendRowData(
    Grid_t const *const grid,
    Line_t *const line,
    int const row,
    bool const off_grid);

static bool _AppendLine(
   Line_t *const line,
   size_t const space_len,
   char const *const str,
   size_t const str_len);

static bool _IsValidLine(
    Line_t *const line);

bool Grid_Init(
   Grid_t *const grid)
{
   bool result = false;
   if (grid)
   {
      grid->rows = MAX_COORD_ROW;
      grid->cols = MAX_COORD_COL;
      memset(grid->ships, 0, SIZE_GRID * sizeof(ShipType_e));
      memset(grid->states, 0, SIZE_GRID * sizeof(GridState_e));
      result = _InitMeta(&grid->meta);
   }
   return result;
}

bool Grid_Clear(
   Grid_t *const grid)
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
    Line_t *const line,
    int const row,
    bool const off_grid)
{
   bool result = false;
   if (grid && _IsValidMeta(&grid->meta) && _IsValidLine(line))
   {
      if (row == GRID_ROW_TITLE)
      {
         result = _AppendRowTitle(grid, line, off_grid);
      }
      else if (row == GRID_ROW_HEADER)
      {
         result = _AppendRowHeader(grid, line);
      }
      else if (row == (int)grid->rows)
      {
         result = _AppendRowFooter(grid, line);

      }
      else
      {
         result = _AppendRowData(grid, line, row, off_grid);
      }
   }
   return result;
}

bool Grid_PlaceShip(
   Grid_t *const grid,
   Ship_t *const player_ship,
   const Ship_t *const ship,
   GridStatus_e *const status)
{
   bool result = false;
   if (grid && ship && status)
   {
      result = _CheckShip(grid, ship, status);
      if (result && GRID_STATUS_OK == *status)
      {
         // Check if ship has been placed before,
         // otherwise location defaults to 0,0 (A1)
         // and may clear a ship already placed there
         if (player_ship->heading != HEADING_UNKNOWN)
         {
            // Remove previous location of ship on grid
            result = _SetShip(grid, grid->ships,
                              player_ship, SHIP_NONE);
         }
         if (result)
         {
            // Place new location of ship on grid
            result = _SetShip(grid, grid->ships,
                              ship, ship->type);
         }
         if (result)
         {
            // Update player ship location and heading
            player_ship->location = ship->location;
            player_ship->heading = ship->heading;
         }
      }
   }
   return result;
}

bool Grid_PlaceHit(
   Grid_t const *const grid,
   GridState_e *const states,
   Coord_t const *const target,
   ShipType_e *const hit_ship)
{
   bool result = false;
   if (grid && states && target && hit_ship)
   {
      bool border = false;
      result = _CheckBorder(grid, target, &border);
      if (result && border)
      {
         int i = target->row * (int)grid->cols + target->col;
         *hit_ship = grid->ships[i];
         states[i] = (*hit_ship != SHIP_NONE) ? GRID_STATE_HIT : GRID_STATE_MISS;
      }
   }
   return result;
}

bool Grid_SinkShip(
    Grid_t const *const grid,
    GridState_e *const states,
    Ship_t const *const ship)
{
   bool result = false;
   if (grid && states && ship)
   {
      result = _SetState(grid, states, ship, GRID_STATE_SUNK);
   }
   return result;
}

bool Grid_ShipTypeToStr(
   ShipType_e const type,
   char const *const str,
   size_t const str_len)
{
   Ship_Info_t const *ship_info = NULL;
   char const *state_str = NULL;
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
   char const *const str,
   size_t const str_len)
{
   bool result = false;
   char const *state_str = NULL;
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
   GridMeta_t *const meta)
{
   bool result = false;
   if (meta)
   {
      meta->row_width = LEN_COORD_ROW;
      meta->col_width = LEN_COORD_COL;

      char * corner_char = STR_GRID_CORNER;
      char * side_char = STR_GRID_SIDE_H;

      bool corner = RepeatChar(
          meta->corner_str,
          LEN_GRID_CORNER + 1,
          corner_char[0]);

      bool side = RepeatChar(
          meta->side_str,
          LEN_GRID_SIDE + 1,
          side_char[0]);

      result = (corner && side);
   }
   return result;
}

bool _IsValidMeta(
   GridMeta_t const *const meta)
{
   bool result = false;
   if (meta)
   {
      result = (
         meta->row_width > 0 &&
         meta->col_width > 0);
   }
   return result;
}

bool _CheckShip(
   Grid_t const *const grid,
   Ship_t const *const ship,
   GridStatus_e *const status)
{
   bool result = false;
   if (grid && ship && status)
   {
      Ship_Info_t const *ship_info = Ship_GetInfo(ship->type);
      if (ship_info)
      {
         *status = GRID_STATUS_OK;
         for (uint i = 0; i < ship_info->length; i++)
         {
            Coord_t loc = Ship_GetPoint(ship, i);
            bool border = false;
            result = _CheckBorder(grid, &loc, &border);
            if (!result) break;
            if (!border)
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
         debug_print("%c %d %d %c %c%c\n",
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

bool _CheckBorder(
   Grid_t const *const grid,
   Coord_t const *const loc,
   bool *const valid)
{
   bool result = false;
   if (grid && loc && valid)
   {
      *valid = (loc->row >= 0 && loc->row < (int)grid->rows &&
                loc->col >= 0 && loc->col < (int)grid->cols);
      result = true;
   }
   return result;
}

bool _SetShip(
   Grid_t const *const grid,
   ShipType_e *const ships,
   Ship_t const *const ship,
   ShipType_e const type)
{
   bool result = false;
   if (ships && ship)
   {
      Ship_Info_t const *const ship_info =
         Ship_GetInfo(ship->type);
      if (ship_info)
      {
         result = true;
         for (uint i = 0; i < ship_info->length; i++)
         {
            Coord_t const loc = Ship_GetPoint(ship, i);
            bool border = false;
            result = _CheckBorder(grid, &loc, &border);
            if (!result) break;
            if (border)
            {
               ships[loc.row * (int)grid->cols + loc.col] = type;
            }
         }
      }
   }
   return result;
}

bool _SetState(
    Grid_t const *const grid,
    GridState_e *const states,
    Ship_t const *const ship,
    GridState_e const state)
{
   bool result = false;
   if (states && ship)
   {
      Ship_Info_t const *const ship_info =
         Ship_GetInfo(ship->type);
      if (ship_info)
      {
         result = true;
         for (uint i = 0; i < ship_info->length; i++)
         {
            Coord_t const loc = Ship_GetPoint(ship, i);
            bool border = false;
            result = _CheckBorder(grid, &loc, &border);
            if (!result) break;
            if (border)
            {
               states[loc.row * (int)grid->cols + loc.col] = state;
            }
         }
      }
   }
   return result;
}

bool _AppendRowTitle(
    Grid_t const *const grid,
    Line_t *const line,
    bool const off_grid)
{
   bool result = false;
   size_t len = 0;
   if (grid && _IsValidMeta(&grid->meta) && _IsValidLine(line))
   {
      // title row
      result = _AppendLine(
          line,
          grid->meta.row_width + LEN_GRID_SPACE,
          (off_grid) ? STR_TITLE_OFFENSE : STR_TITLE_DEFENSE,
          LEN_TITLE_STR);
      // row filler
      len = LEN_GRID_SIDE + LEN_GRID_CORNER - LEN_TITLE_STR;
      result = RepeatChar(
          line->buffer + line->position,
          len,
          ' ');
      if (result)
      {
         line->position += len - 1;
      }
   }
   return result;
}

bool _AppendRowHeader(
    Grid_t const *const grid,
    Line_t *const line)
{
   bool result = false;
   char cell_str[SIZE_CELL_STR] = {0};
   if (grid && _IsValidMeta(&grid->meta) && _IsValidLine(line))
   {
      // header row
      // grid corner
      result = _AppendLine(
          line,
          grid->meta.row_width - LEN_GRID_CORNER,
          grid->meta.corner_str,
          LEN_GRID_CORNER);
      if (result)
      {
         // column labels
         for (uint col = 0; col < grid->cols; col++)
         {
            result = Coord_ColToChar(
                (int)col,
                cell_str);
            if (!result)
               break;
            result = _AppendLine(
                line,
                LEN_GRID_SPACE,
                cell_str,
                grid->meta.col_width);
            if (!result)
               break;
         }
      }
      if (result)
      {
         // grid corner
         result = _AppendLine(
             line,
             LEN_GRID_SPACE,
             STR_GRID_CORNER,
             LEN_GRID_SPACE);
      }
   }
   return result;
}

bool _AppendRowFooter(
    Grid_t const *const grid,
    Line_t *const line)
{
   bool result = false;
   if (grid && _IsValidMeta(&grid->meta) && _IsValidLine(line))
   {
      // grid corner
      result = _AppendLine(
          line,
          grid->meta.row_width - LEN_GRID_CORNER,
          grid->meta.corner_str,
          LEN_GRID_CORNER);
      if (result)
      {
         // row filler
         result = _AppendLine(
             line,
             LEN_GRID_SPACE,
             grid->meta.side_str,
             LEN_GRID_SIDE - 1);
      }
      if (result)
      {
         // grid corner
         result = _AppendLine(
             line,
             LEN_GRID_SPACE,
             STR_GRID_CORNER,
             LEN_GRID_SPACE);
      }
   }
   return result;
}

bool _AppendRowData(
    Grid_t const *const grid,
    Line_t *const line,
    int const row,
    bool const off_grid)
{
   bool result = false;
   char cell_str[SIZE_CELL_STR] = {0};
   ShipType_e ship = SHIP_NONE;
   GridState_e state = GRID_STATE_BLANK;
   if (grid && _IsValidMeta(&grid->meta) && _IsValidLine(line))
   {
      // numbered rows
      // row label
      if (snprintf(
              line->buffer + line->position,
              MAX_BUFFER_SIZE - line->position,
              "%*u",
              (int)grid->meta.row_width,
              row + 1) > 0)
      {
         line->position += (grid->meta.row_width);
         result = true;
      }
      if (result)
      {
         // cell states
         for (int col = 0; col < (int)grid->cols; col++)
         {
            int i = row * (int)(grid->cols) + col;
            state = grid->states[i];
            ship = grid->ships[i];
            // show ship type if not hit or sunk on defense grid
            // only reveal ship type on offense grid when sunk
            if ((!off_grid && GRID_STATE_BLANK == state) ||
                (off_grid && GRID_STATE_SUNK == state))
            {
               Grid_ShipTypeToStr(
                  ship, cell_str, SIZE_CELL_STR);
            }
            else
            {
               Grid_StateToStr(
                  state, cell_str, SIZE_CELL_STR);
            }
            result = _AppendLine(
                line,
                LEN_GRID_SPACE,
                cell_str,
                grid->meta.col_width);
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
             LEN_GRID_SPACE,
             STR_GRID_SIDE_V,
             LEN_GRID_SPACE);
      }
   }
   return result;
}

bool _AppendLine(
    Line_t *const line,
    size_t const space_len,
    char const *const str,
    size_t const str_len)
{
   bool result = false;
   if (_IsValidLine(line) &&
      space_len < MAX_BUFFER_SIZE &&
      str_len < MAX_BUFFER_SIZE)
   {
      if (snprintf(
              line->buffer + line->position,
              MAX_BUFFER_SIZE - line->position,
              "%*s%*s",
              (int)space_len,
              "",
              (int)str_len,
              str) > 0)
      {
         line->position += (space_len + str_len);
         result = true;
      }
   }
   return result;
}

bool _IsValidLine(
    Line_t *const line)
{
   return (line && line->position < MAX_BUFFER_SIZE);
}
