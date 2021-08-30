/*
 * ui_stdio.c
 *
 *  Created on: 9 April 2021
 *      Author: pejdavies1606
 */

#include <stdio.h>
#include <limits.h>
#include "battleship/util.h"
#include "battleship/ui.h"
#include "battleship/input.h"

// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
// Borland-style CONIO implementation for MinGW/Dev-C++ (http://conio.sourceforge.net/)
#if defined(CONIO)
#include "conio21/conio2.h"
#endif

static bool BattleShip_UI_Read(
   String_t prompt,
   size_t option_len,
   uint option_max,
   InputData_t *data);

static bool ReadString(String_t str, size_t str_size, FILE *stream);

void BattleShip_UI_Clear_Screen(void)
{
#ifdef DEBUG
   puts("clrscr");
#else
#if defined(CONIO)
   clrscr();
#else
   printf("\e[1;1H\e[2J")
#endif
#endif
}

void BattleShip_UI_Print_Logo(void)
{
#ifdef DEBUG
   puts("logo");
#else
   // ASCII font 'cybermedium' http://www.topster.de/text-to-ascii/cybermedium.html
   puts("                       |                           ");
   puts("                       |                           ");
   puts("                 .  -  +  -  .                     ");
   puts("              +'       |       '+                  ");
   puts("            +          |          +                ");
   puts(" ___  ____+___ ___ _   |____ ____ _ +_ _ ___  ____ ");
   puts(" |__] |__|  |   |  |   ||___ [__  |__| | |__] [__  ");
   puts(" |__] | /|  |   |  |___||___ ___] |  |\\| |    ___] ");
   puts("       '               |               '           ");
   puts("       |             _ |_  _           |           ");
   puts("   ----+-------------|-+|\\ |-----------+----       ");
   puts("       |             | || \\|           |           ");
   puts("       .            _______            .           ");
   puts("       \\            |  |               /           ");
   puts("        \\           |  |              /            ");
   puts("          +         |__|___         +              ");
   puts("            +          |          +                ");
   puts("              +.       |       .+                  ");
   puts("                 '  -  +  -  '                     ");
   puts("                       |                           ");
   puts("                       |                           ");
#endif
}

void BattleShip_UI_Print_Message(String_t message)
{
   puts(message);
}

Status_t BattleShip_UI_Print_Grid_Defense(const Grid_t *grid)
{
   Status_t result = STATUS_ERROR;
   Grid_State_t *defense = NULL;
   const Grid_Meta_t *grid_meta = NULL;

   if (grid)
   {
      defense = grid->defense;
      grid_meta = &grid->meta;

      if (defense && grid_meta)
      {
         char ship_str[SIZE_STATE_STR + 1];
         char state_str[SIZE_STATE_STR + 1];
         memset(ship_str, 0, sizeof(ship_str));
         memset(state_str, 0, sizeof(state_str));

         printf("%*s%s\n",
               (int)(grid_meta->row_width - 1), "",
               STR_DEF);

         for (int row = -1; row < (int)(grid->rows + 1); row++)
         {
            if (row == -1)
            {
               // style choice: SIZE_GRID_SPACE instead of corner_len
               printf("%*s%.*s", (int)(grid_meta->row_width - 1), "",
                     SIZE_GRID_SPACE, grid_meta->corner_str);
               for (uint col = 0; col < grid->cols; col++)
               {
                  printf("%*s%-*c", SIZE_GRID_SPACE, "", 
                        (int)grid_meta->col_width, col + 65); // ASCII
               }
               printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_CORNER);
            }
            else if(row == (int) grid->rows)
            {
               // style choice: SIZE_GRID_SPACE instead of corner_len
               printf("%*s%.*s", (int)(grid_meta->row_width - 1), "",
                     SIZE_GRID_SPACE, grid_meta->corner_str);
               printf("%*s%.*s", SIZE_GRID_SPACE, "",
                     (int)grid_meta->side_len, grid_meta->side_str);
               printf("%*s%s\n", SIZE_GRID_SPACE, "",
                     STR_GRID_CORNER);
            }
            else
            {
               printf("%*u", (int)grid_meta->row_width, row + 1);
               for (int col = 0; col < (int) grid->cols; col++)
               {
                  Grid_State_t p = defense[row*(int)(grid->cols) + col];
                  ShipTypeToStr(p.ship_type, ship_str, sizeof(ship_str));
                  HitStateToStr(p.hit_state, state_str, sizeof(state_str));
                  printf("%*s%*s",
                        SIZE_GRID_SPACE, "",
                        (int)grid_meta->col_width,
                        (p.hit_state == STATE_HIT) ? state_str : ship_str);
               }
               printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_SIDE_V);
            }
         }
         result = STATUS_OK;
      }
   }
   return result;
}

/*void BattleShip_UI_Print_Grid(const Hit_State_t *offense)
{
}*/

/*void BattleShip_UI_Print_Grid(const Grid_State_t *defense, const Hit_State_t *offense)
{
}*/

void BattleShip_UI_Print_Menu(Menu_t *menu)
{
   puts("");
   printf("%*s%s\n", menu->meta.column_width_index, "", menu->title);
   printf("%*s%s", menu->meta.column_width_index, "", "#");
   for (uint col=0; col < menu->num_headers; col++)
   {
      uint column_width = (col == 0) ? 1 :
         (menu->meta.column_width_data[col-1] - menu->meta.header_width_data[
          (col-1)
         ] + 1);
      printf("%*s%s", column_width, "",
            IF_NULL_VAL(menu->headers[col],"")
            );
   }
   printf("\n");
   for (uint row=0; row < menu->num_options; row++)
   {
      printf("%*s%d", menu->meta.column_width_index, "", row);
      for (uint col=0; col < menu->num_headers; col++)
      {
         uint column_width = (col == 0) ? 1 :
            (menu->meta.column_width_data[col-1] - menu->meta.option_width_data[
             row + (col-1)*menu->num_options
            ] + 1);
         printf("%*s%s", column_width, "",
               IF_NULL_VAL(menu->options[row + col*menu->num_options],"")
               );
      }
      printf("\n");
   }
}

bool BattleShip_UI_Read_Menu(Menu_t *menu, uint *choice)
{
   bool result = false;
   InputData_t option = { 0 };
   if (menu && choice)
   {
      option.type = INPUT_ULONG;
      result = BattleShip_UI_Read(
          "option",
          menu->meta.column_width_index,
          menu->num_options,
          &option);
      if (result)
      {
         *choice = option.val.ul;
      }
   }
   return result;
}

bool BattleShip_UI_Read_Ship_Location_Heading(Coord_t *location, Heading_e *heading)
{
   bool result = false;
   bool parse_success[3] = { false };
   InputData_t col = { 0 };
   InputData_t row = { 0 };
   InputData_t hdg = { 0 };
   int len = CalcNumWidth(GRID_SIZE);
   if (location && heading && len > 0)
   {
      col.type = INPUT_ULONG;
      row.type = INPUT_ULONG;
      hdg.type = INPUT_HEADING;
      //printf("Enter %s: ", "<0+> <0+> <0+>");
      //parse_success = (3 == sscanf(buf, "%2d %2d %1u", &location->col, &location->row, heading));
      parse_success[0] = BattleShip_UI_Read(
         "col <A-J>",
         (size_t) len,
         GRID_SIZE,
         &col);
      parse_success[1] = BattleShip_UI_Read(
         "row <1-10>",
         (size_t) len,
         GRID_SIZE,
         &row);
      parse_success[2] = BattleShip_UI_Read(
         "hdg <N|E|S|W>",
         LEN_HEADING,
         NUM_HEADINGS,
         &hdg);
      result = (
         parse_success[0] &&
         parse_success[1] && 
         parse_success[2]);
      if (result)
      {
         location->col = (int) col.val.ul;
         location->row = (int) row.val.ul;
         *heading = (Heading_e) hdg.val.hdg;
         printf("%d %d %u\n", location->col, location->row, *heading);
      }
   }
   return result;
}

bool BattleShip_UI_Read(
   String_t prompt,
   size_t option_len,
   uint option_max,
   InputData_t *data)
{
   size_t chosen_option_len = option_len + 1;
   String_t chosen_option_str = malloc( (chosen_option_len) * sizeof(char) );
   uint chosen_option = UINT_MAX;
   uint retries = 0;
   bool result = false;
   if (data)
   {
      if (option_max == 0)
      {
         option_max = UINT_MAX;
      }
      while (!result && retries < MAX_READ_RETRIES)
      {
#ifdef DEBUG
         printf("%d Enter %s: ", retries, prompt);
#else
         printf("Enter %s: ", prompt);
#endif
         result = ReadString(
            chosen_option_str,
            chosen_option_len,
            stdin);
         if (result)
         {
            result = ParseInput(
               chosen_option_str,
               data);
            if (result && data->type == INPUT_ULONG)
            {
               // TODO move into input.c
               // use InputData_t::min/max
               result = (data->val.ul < option_max);
            }
         }
         if (!result)
         {
            retries++;
         }
      }
      free(chosen_option_str);
   }
   return result;
}

bool ReadString(String_t str, size_t str_size, FILE *stream)
{
   bool result = false;
   // Type-ahead cannot be avoided by attempting to 'flush' stdin.
   // http://c-faq.com/stdio/gets_flush2.html
   // http://c-faq.com/stdio/stdinflush2.html
   //int c; while ((c = getchar()) != '\n' && c != EOF);
   if (str && str_size > 0 && stream)
   {
      memset(str, 0, str_size);
      size_t line_size = str_size + 1; // to account for newline from fgets
      String_t line = malloc(line_size * sizeof(*line));
      if (line)
      {
         if (fgets(line, (int)line_size, stream))
         {
            line = TrimStr(line, line_size);
            strncpy(str, line, str_size);
            free(line);
            result = true;
         }
      }
   }
   return result;
}
