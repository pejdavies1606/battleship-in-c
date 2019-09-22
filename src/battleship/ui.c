/*
 * ui.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ship.h"
#include "grid.h"
#include "util.h"
#include "ui.h"

#ifndef NDEBUG
#define clrscr() puts("\n-----clrscr-----")
#else
// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
#if defined(MSDOS) && MSDOS == 1
// Borland-style CONIO implementation for MinGW/Dev-C++ (http://conio.sourceforge.net/)
#include "../conio21/conio2.h"
#else
#define clrscr() printf("\e[1;1H\e[2J")
#endif
#endif

#define MAX_READ_RETRIES 3

#define NUM_MAIN_MENU_HEADERS 1
#define NUM_PLACE_MENU_HEADERS 1
#define NUM_SHIP_MENU_HEADERS 2

#define IF_NULL_BLANK(s) ( (NULL == s) ? "" : s)

static String_t main_menu_headers[NUM_MAIN_MENU_HEADERS] =
{
   "Option"
};

static String_t main_menu_options[NUM_MAIN_MENU_OPTIONS] =
{
   "Quit",
   "Place your ships",
   "Begin battle!",
   "Watch an automated battle",
   "About"
};

static Menu_t main_menu =
{
   .title = "Main Menu",
   .num_headers = NUM_MAIN_MENU_HEADERS,
   .headers = main_menu_headers,
   .num_options = NUM_MAIN_MENU_OPTIONS,
   .options = main_menu_options
};

static String_t place_menu_headers[NUM_PLACE_MENU_HEADERS] =
{
   "Option",
};

static String_t place_menu_options[NUM_PLACE_MENU_OPTIONS] =
{
   "Return",
   "Help",
   "Automatic",
   "Manual"
};

static Menu_t place_menu =
{
   .title = "Ship Placement Menu",
   .num_headers = NUM_PLACE_MENU_HEADERS,
   .headers = place_menu_headers,
   .num_options = NUM_PLACE_MENU_OPTIONS,
   .options = place_menu_options
};

static String_t ship_menu_headers[NUM_SHIP_MENU_HEADERS] =
{
   "Option",
   "Length"
};

// stored column-wise, printed row-wise
static String_t ship_menu_options[NUM_SHIP_MENU_OPTIONS*NUM_SHIP_MENU_HEADERS] =
{
   "Return", "Place",
   "L0", "L1",
};

static Menu_t ship_menu =
{
   .title = "Ships",
   .num_headers = NUM_SHIP_MENU_HEADERS,
   .headers = ship_menu_headers,
};

static Grid_Meta_t grid_meta;

void BattleShip_UI_Init(void)
{
   Menu_Meta_Init( &main_menu );
   Menu_Meta_Init( &place_menu );

   // complete ship menu at runtime
   {
      size_t num_options = NUM_SHIPS + 1;
      String_t *ship_menu_data = malloc(sizeof(String_t) * num_options * NUM_SHIP_MENU_HEADERS);
      String_t return_str = ship_menu_options[MENU_OPTION_SHIP_RETURN];
      String_t place_prefix = ship_menu_options[MENU_OPTION_SHIP_PLACE];

      for (uint i=0; i<num_options; i++)
      {
         uint name_index = i;
         uint length_index = i + num_options;

         if ( i == 0 )
         {
            size_t return_len = strlen(return_str) + 1;
            ship_menu_data[name_index] = malloc(return_len);
            snprintf(ship_menu_data[name_index], return_len, "%s", return_str);

            ship_menu_data[length_index] = ""; // blank entry
         }
         else
         {
            String_t ship_name = SHIP_NAME[i-1];

            size_t name_len = strlen(place_prefix) + 1 + strlen(ship_name) + 1;
            size_t length_len = (size_t)CalcNumWidth(CalcMax((int*)SHIP_LENGTH, NUM_SHIPS)) + 1;

            ship_menu_data[name_index] = malloc(name_len);
            ship_menu_data[length_index] = malloc(length_len);

            snprintf(ship_menu_data[name_index], name_len, "%s %s", place_prefix, ship_name);
            snprintf(ship_menu_data[length_index], length_len, "%u", SHIP_LENGTH[i-1]);
         }
      }

      // redefine template options table in menu with new, completed options table
      ship_menu.num_options = num_options;
      ship_menu.options = ship_menu_data;
   }

   Menu_Meta_Init( &ship_menu );

   Grid_Meta_Init( &grid_meta, GRID_SIZE );
}

void BattleShip_UI_Print_Logo(void)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
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

void ShipType2Str(const Ship_Type_t type, char *str, const size_t str_len)
{
   switch(type)
   {
      case SHIP_DESTROYER:
      case SHIP_CRUISER:
      case SHIP_SUBMARINE:
      case SHIP_BATTLESHIP:
      case SHIP_AIRCRAFT_CARRIER:
         snprintf(str, str_len, "%c", SHIP_NAME[type][0]);
         break;
      case SHIP_NONE:
         snprintf(str, str_len, "%s", STR_STATE_BLANK);
         break;
   }
}

void HitState2Str(const Hit_State_t state, char *str, size_t str_len)
{
   switch(state)
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

void BattleShip_UI_Print_Defense_Grid(const Grid_State_t *defense)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   char ship_str[SIZE_STATE_STR + 1];
   char state_str[SIZE_STATE_STR + 1];

   printf("%*s%s\n", (GRID_SIZE/2)*(grid_meta.col_width + SIZE_GRID_SPACE), "",
      STR_DEF);
   for (int row = -1; row < GRID_SIZE+1; row++)
   {
      if (row == -1)
      {
         // style choice: SIZE_GRID_SPACE instead of corner_len
         printf("%*s%.*s", grid_meta.col_width - 1, "",
            SIZE_GRID_SPACE, grid_meta.corner_str);
         for (uint col = 0; col < GRID_SIZE; col++)
         {
            printf("%*s%-*c", SIZE_GRID_SPACE, "", 
               grid_meta.col_width, col + 65); // ASCII
         }
         printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_CORNER);
      }
      else if(row == GRID_SIZE)
      {
         // style choice: SIZE_GRID_SPACE instead of corner_len
         printf("%*s%.*s", grid_meta.col_width - 1, "",
            SIZE_GRID_SPACE, grid_meta.corner_str);
         printf("%*s%.*s", SIZE_GRID_SPACE, "",
            grid_meta.side_len, grid_meta.side_str);
         printf("%*s%s\n", SIZE_GRID_SPACE, "",
            STR_GRID_CORNER);
      }
      else
      {
         printf("%*u", grid_meta.col_width, row + 1);
         for (int col = 0; col < GRID_SIZE; col++)
         {
            Grid_State_t p = defense[row*GRID_SIZE + col];
            ShipType2Str(p.ship_type, ship_str, strlen(ship_str));
            HitState2Str(p.hit_state, state_str, strlen(state_str));
            printf("%*s%s%s", SIZE_GRID_SPACE, "", ship_str, state_str);
         }
         printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_SIDE_V);
      }
   }
}

/*void BattleShip_UI_Print_Grid(const Hit_State_t *offense)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
}*/

/*void BattleShip_UI_Print_Grid(const Grid_State_t *defense, const Hit_State_t *offense)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
}*/

Main_Menu_Option_t BattleShip_UI_Main_Menu(String_t message)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   Main_Menu_Option_t choice = MENU_OPTION_MAIN_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      BattleShip_UI_Print_Logo();
      puts(message);
      BattleShip_UI_Print_Menu(&main_menu);
      read_success = BattleShip_UI_Read_Menu(&main_menu, (uint*) &choice);
#ifndef NDEBUG
      printf("read=%s\n", read_success ? "true" : "false");
#endif
   }
   return choice;
}

Place_Menu_Option_t BattleShip_UI_Place_Menu(const Grid_State_t *defense)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   Place_Menu_Option_t choice = MENU_OPTION_PLACE_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      BattleShip_UI_Print_Defense_Grid(defense);
      BattleShip_UI_Print_Menu(&place_menu);
      read_success = BattleShip_UI_Read_Menu(&place_menu, (uint*) &choice);
   }
   return choice;
}

Ship_Menu_Option_t BattleShip_UI_Ship_Menu(const Grid_State_t *defense)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   Ship_Menu_Option_t choice = MENU_OPTION_SHIP_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      BattleShip_UI_Print_Defense_Grid(defense);
      BattleShip_UI_Print_Menu(&ship_menu);
      read_success = BattleShip_UI_Read_Menu(&ship_menu, (uint*) &choice);
   }
   return choice;
}

void BattleShip_UI_Print_Menu(Menu_t *menu)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
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
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   size_t chosen_option_len = menu->meta.column_width_index + 1;
   String_t chosen_option_str = malloc( (chosen_option_len) * sizeof(*chosen_option_str) );
   uint chosen_option = menu->num_options;
   int retries = 0;
   bool parse_success = false;
   while (!parse_success && retries < MAX_READ_RETRIES)
   {
#ifndef NDEBUG
      printf("[%d]Enter option: ", retries);
#else
      printf("Enter option: ");
#endif
      ReadString(chosen_option_str, (int)chosen_option_len, stdin);
      parse_success = ParseUnsignedLong(chosen_option_str, (unsigned long*) &chosen_option);
      if (chosen_option >= menu->num_options) parse_success = false;
      if (!parse_success) retries++;
   }
   free(chosen_option_str);
   *choice = chosen_option;
   return !(retries == MAX_READ_RETRIES);
}
