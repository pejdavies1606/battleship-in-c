/*
 * ui_stdio.c
 *
 *  Created on: 9 April 2021
 *      Author: pejdavies1606
 */

#include <stdio.h>
#include <limits.h>
#include "battleship/ui.h"

// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
// Borland-style CONIO implementation for MinGW/Dev-C++ (http://conio.sourceforge.net/)
#if defined(MSDOS) && MSDOS == 1
#include "conio21/conio2.h"
#endif

void BattleShip_UI_Clear_Screen(void)
{
#ifdef DEBUG
   puts("clrscr");
#else
#if defined(MSDOS) && MSDOS == 1
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

// TODO replace GRID_SIZE with grid params
Status_t BattleShip_UI_Print_Grid_Defense(const Grid_t *grid)
{
   Grid_State_t *defense = grid->defense;
   Grid_Meta_t *grid_meta = BattleShip_UI_Get_Grid_Meta();

   if (!defense || !grid_meta)
   {
      return STATUS_ERROR;
   }
   char ship_str[SIZE_STATE_STR + 1];
   char state_str[SIZE_STATE_STR + 1];
   memset(ship_str, 0, sizeof(ship_str));
   memset(state_str, 0, sizeof(state_str));

   printf("%*s%s\n",
      (int)(grid_meta->row_width - 1), "",
      STR_DEF);

   for (int row = -1; row < GRID_SIZE+1; row++)
   {
      if (row == -1)
      {
         // style choice: SIZE_GRID_SPACE instead of corner_len
         printf("%*s%.*s", (int)(grid_meta->row_width - 1), "",
            SIZE_GRID_SPACE, grid_meta->corner_str);
         for (uint col = 0; col < GRID_SIZE; col++)
         {
            printf("%*s%-*c", SIZE_GRID_SPACE, "", 
               (int)grid_meta->col_width, col + 65); // ASCII
         }
         printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_CORNER);
      }
      else if(row == GRID_SIZE)
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
         for (int col = 0; col < GRID_SIZE; col++)
         {
            Grid_State_t p = defense[row*GRID_SIZE + col];
            ShipType2Str(p.ship_type, ship_str, sizeof(ship_str));
            HitState2Str(p.hit_state, state_str, sizeof(state_str));
            printf("%*s%*s",
                  SIZE_GRID_SPACE, "",
                  (int)grid_meta->col_width,
                  (p.hit_state == STATE_HIT) ? state_str : ship_str);
         }
         printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_SIDE_V);
      }
   }
   return STATUS_OK;
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
   size_t chosen_option_len = menu->meta.column_width_index + 1;
   String_t chosen_option_str = malloc( (chosen_option_len) * sizeof(*chosen_option_str) );
   uint chosen_option = menu->num_options;
   int retries = 0;
   bool parse_success = false;
   while (!parse_success && retries < MAX_READ_RETRIES)
   {
#ifndef NDEBUG
      printf("%d Enter option: ", retries);
#else
      printf("Enter option: ");
#endif
      ReadString(chosen_option_str, chosen_option_len, stdin);
      parse_success = ParseUnsignedLong(chosen_option_str, (unsigned long*) &chosen_option);
      if (chosen_option >= menu->num_options) parse_success = false;
      if (!parse_success) retries++;
   }
   free(chosen_option_str);
   *choice = chosen_option;
   return !(retries == MAX_READ_RETRIES);
}

bool BattleShip_UI_Read_Ship_Location_Heading(Coord_t *location, Heading_t *heading)
{
   if (!location || !heading)
   {
      return false;
   }
   //printf("Enter %s: ", "<A-J> <1-10> <N|E|S|W>");
   printf("Enter %s: ", "<0+> <0+> <0+>");
   char buf[8];
   ReadString(buf, sizeof(buf), stdin);
   bool parse_success = (3 == sscanf(buf, "%2d %2d %1u", &location->col, &location->row, heading));
   printf("%d %d %u\n", location->col, location->row, *heading);
   /*parse_success |= BattleShip_UI_Read("col", (size_t) CalcNumWidth((int) GRID_SIZE), GRID_SIZE,
         (uint*) &location->col, NULL);
   parse_success |= BattleShip_UI_Read("row", (size_t) CalcNumWidth((int) GRID_SIZE), GRID_SIZE,
         (uint*) &location->row, NULL);*/
   /*parse_success |= BattleShip_UI_Read("hdg", (size_t) LEN_HEADING, NUM_HEADINGS,
         (uint*) &heading, &ValidateHeading);*/
   return parse_success;
}

static int BattleShip_UI_Read(
   String_t prompt,
   size_t option_len,
   uint option_max,
   uint *choice,
   InputParser_t InputParser)
{
   if (!choice)
   {
      return false;
   }
   size_t chosen_option_len = option_len + 1;
   String_t chosen_option_str = malloc( (chosen_option_len) * sizeof(*chosen_option_str) );
   uint chosen_option = UINT_MAX;
   if (option_max == 0)
   {
      option_max = UINT_MAX;
   }
   int retries = 0;
   bool parse_success = false;
   while (!parse_success && retries < MAX_READ_RETRIES)
   {
#ifndef NDEBUG
      printf("%d Enter %s: ", retries, prompt);
#else
      printf("Enter %s: ", prompt);
#endif
      ReadString(chosen_option_str, chosen_option_len, stdin);
      if (InputParser)
      {
         parse_success = InputParser(chosen_option_str, (unsigned long*) &chosen_option);
      }
      else
      {
         parse_success = ParseUnsignedLong(chosen_option_str, (unsigned long*) &chosen_option);
      }
      if (chosen_option >= option_max)
      {
         parse_success = false;
      }
      if (!parse_success)
      {
         retries++;
      }
   }
   free(chosen_option_str);
   if (InputParser)
   {
      *choice = chosen_option;
   }
   return !(retries == MAX_READ_RETRIES);
}

