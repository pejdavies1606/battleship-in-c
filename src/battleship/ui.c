/*
 * ui.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ui.h"
#include "ship.h"
#include "util.h"

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

#define SIZE_SHIP_SYMBOLS 3
#define MAX_READ_RETRIES 3

#define NUM_MAIN_MENU_HEADERS 1
#define NUM_PLACE_MENU_HEADERS 1
#define NUM_SHIP_MENU_HEADERS 3

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
   "Symbol",
   "Length"
};

// stored column-wise, printed row-wise
static String_t ship_menu_options[NUM_SHIP_MENU_OPTIONS*NUM_SHIP_MENU_HEADERS] =
{
   "Return", "Place",
   "S0", "S1",
   "L0", "L1",
};

static Menu_t ship_menu =
{
   .title = "Ships",
   .num_headers = NUM_SHIP_MENU_HEADERS,
   .headers = ship_menu_headers,
   .num_options = NUM_SHIP_MENU_OPTIONS,
   .options = ship_menu_options
};

void BattleShip_UI_Print_Logo()
{
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
}

void BattleShip_UI_Print_Grid(const Grid_State_t *defense)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
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
   Menu_Meta_t meta;
   Menu_Meta_Init( &meta, &main_menu );

   Main_Menu_Option_t choice = MENU_OPTION_MAIN_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      BattleShip_UI_Print_Logo();
      puts(message);
      BattleShip_UI_Print_Menu(&main_menu, &meta);
      read_success = BattleShip_UI_Read_Menu(&main_menu, &meta, (uint*) &choice);
#ifndef NDEBUG
      printf("read=%s\n", read_success ? "true" : "false");
#endif
   }
   return choice;
}

Place_Menu_Option_t BattleShip_UI_Place_Menu()
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   Menu_Meta_t meta;
   Menu_Meta_Init( &meta, &place_menu );

   Place_Menu_Option_t choice = MENU_OPTION_PLACE_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      //BattleShip_UI_Print_Grid(); // TODO
      BattleShip_UI_Print_Menu(&place_menu, &meta);
      read_success = BattleShip_UI_Read_Menu(&place_menu, &meta, (uint*) &choice);
   }
   return choice;
}

Ship_Menu_Option_t BattleShip_UI_Ship_Menu(const Grid_State_t *defense)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif

   // define new options table
   size_t num_options = NUM_SHIPS + 1;
   String_t *ship_menu_data = malloc(sizeof(String_t) * num_options * NUM_SHIP_MENU_HEADERS);

   String_t return_str = ship_menu_options[MENU_OPTION_SHIP_RETURN];
   size_t return_len = strlen(return_str) + 1;

   ship_menu_data[0] = malloc(return_len);
   ship_menu_data[1] = malloc(1);
   ship_menu_data[2] = malloc(1);

   snprintf(ship_menu_data[0], return_len, "%s", return_str);
   snprintf(ship_menu_data[1], 1, "");
   snprintf(ship_menu_data[2], 1, "");

   // use template options table to complete new options table
   String_t place_prefix = ship_menu_options[MENU_OPTION_SHIP_PLACE];

   for (uint i=1; i<num_options; i++)
   {
      uint name_index = i;
      uint symbol_index = i + 1*num_options;
      uint length_index = i + 2*num_options;

      size_t name_len = strlen(place_prefix) + 1 + strlen(SHIP_NAME[i-1]) + 1;
      size_t symbol_len = SIZE_SHIP_SYMBOLS + 1;
      size_t length_len = strlen(SHIP_NAME[i-1]) + 1;

      ship_menu_data[name_index] = malloc(name_len);
      ship_menu_data[symbol_index] = malloc(symbol_len);
      ship_menu_data[length_index] = malloc(length_len);

      snprintf(ship_menu_data[name_index], name_len, "%s %s", place_prefix, SHIP_NAME[i-1]);
      snprintf(ship_menu_data[symbol_index], symbol_len, "%c %c", SHIP_NAME[i-1][0], tolower(SHIP_NAME[i-1][0]));
      snprintf(ship_menu_data[length_index], length_len, "%u", SHIP_LENGTH[i-1]);
   }

   // redefine template options table in menu with new, completed options table
   ship_menu.num_options = num_options;
   ship_menu.options = ship_menu_data;

   Menu_Meta_t meta;
   Menu_Meta_Init( &meta, &ship_menu );

   Ship_Menu_Option_t choice = MENU_OPTION_SHIP_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      BattleShip_UI_Print_Grid(defense);
      BattleShip_UI_Print_Menu(&ship_menu, &meta);
      read_success = BattleShip_UI_Read_Menu(&ship_menu, &meta, (uint*) &choice);
   }
   return choice;
}

void BattleShip_UI_Print_Menu(Menu_t *menu, Menu_Meta_t *meta)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   puts("");
   printf("%*s%s\n", meta->column_width_index, "", menu->title);
   printf("%*s%s", meta->column_width_index, "", "#");
   for (uint col=0; col < menu->num_headers; col++)
   {
      uint column_width = (col == 0) ? 1 :
         (meta->column_width_data[col-1] - meta->header_width_data[
          (col-1)
         ] + 1);
      printf("%*s%s", column_width, "",
            IF_NULL_VAL(menu->headers[col],"")
            );
   }
   printf("\n");
   for (uint row=0; row < menu->num_options; row++)
   {
      printf("%*s%d", meta->column_width_index, "", row);
      for (uint col=0; col < menu->num_headers; col++)
      {
         uint column_width = (col == 0) ? 1 :
            (meta->column_width_data[col-1] - meta->option_width_data[
             row + (col-1)*menu->num_options
            ] + 1);
         printf("%*s%s", column_width, "",
               IF_NULL_VAL(menu->options[row + col*menu->num_options],"")
               );
      }
      printf("\n");
   }
}

bool BattleShip_UI_Read_Menu(Menu_t *menu, Menu_Meta_t *meta, uint *choice)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   size_t chosen_option_len = meta->column_width_index + 1;
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
      ReadString(chosen_option_str, chosen_option_len, stdin);
      parse_success = ParseUnsignedLong(chosen_option_str, (unsigned long*) &chosen_option);
      if (chosen_option >= menu->num_options) parse_success = false;
      if (!parse_success) retries++;
   }
   free(chosen_option_str);
   *choice = chosen_option;
   return !(retries == MAX_READ_RETRIES);
}
