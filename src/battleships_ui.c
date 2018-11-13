/*
 * battleships_print.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "battleships_ui.h"
#include "battleships_ship.h"
#include "util.h"

// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
#ifdef _WIN32
// Borland-style CONIO implementation for MinGW/Dev-C++ (http://conio.sourceforge.net/)
#include "conio21/conio2.h"
#else
#define clrscr() printf("\e[1;1H\e[2J")
#endif

#define SIZE_SHIP_SYMBOLS 3
#define MAX_READ_RETRIES 3

#define NUM_MAIN_MENU_HEADERS 1
#define NUM_PLACE_MENU_HEADERS 1
#define NUM_SHIP_MENU_HEADERS 3

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

static String_t ship_menu_options[NUM_SHIP_MENU_OPTIONS*NUM_SHIP_MENU_HEADERS] =
{
      "Return", "S0", "L0",
      "Place",  "S1", "L1"
};

static Menu_t ship_menu =
{
      .title = "Ships",
      .num_headers = NUM_SHIP_MENU_HEADERS,
      .headers = ship_menu_headers,
      .num_options = NUM_SHIP_MENU_OPTIONS,
      .options = ship_menu_options
};

void Battleships_UI_Print_Logo()
{
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

Main_Menu_Option_t Battleships_UI_Main_Menu(String_t message)
{
#ifdef DEBUG
   puts("Battleships_UI_Main_Menu");
#endif
   Menu_Meta_t meta;
   Menu_Meta_Init( &meta, &main_menu );

   Main_Menu_Option_t choice = MENU_OPTION_MAIN_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      //clrscr();
      //Battleships_UI_Print_Logo();
      puts(message);
      Battleships_UI_Print_Menu(&main_menu, &meta);
      read_success = Battleships_UI_Read_Menu(&main_menu, &meta, (uint*) &choice);
      printf("read=%s\n", read_success ? "true" : "false");
   }
   return choice;
}

Place_Menu_Option_t Battleships_UI_Place_Menu()
{
#ifdef DEBUG
   puts("Battleships_UI_Place_Menu");
#endif
   Menu_Meta_t meta;
   Menu_Meta_Init( &meta, &place_menu );

   Place_Menu_Option_t choice = MENU_OPTION_PLACE_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      //clrscr();
      //Battleships_UI_Print_Grid(); // TODO
      Battleships_UI_Print_Menu(&place_menu, &meta);
      read_success = Battleships_UI_Read_Menu(&place_menu, &meta, (uint*) &choice);
   }
   return choice;
}

Ship_Menu_Option_t Battleships_UI_Ship_Menu()
{
#ifdef DEBUG
   puts("Battleships_UI_Ship_Menu");
#endif

   String_t *ship_menu_data = malloc(sizeof(String_t) * NUM_SHIPS * NUM_SHIP_MENU_HEADERS);

   String_t place_prefix = ship_menu_options[MENU_OPTION_SHIP_PLACE];

   for (uint i=0; i<NUM_SHIPS; i++)
   {
      uint name_index = i;
      uint symbol_index = i + NUM_SHIPS;
      //uint length_index = i + NUM_SHIPS*2;

      ship_menu_data[name_index] = malloc(sizeof place_prefix + 1 + sizeof SHIP_NAME[i] + 1);
      ship_menu_data[symbol_index] = malloc(SIZE_SHIP_SYMBOLS + 1);
      //ship_menu_data[length_index] = malloc(1);

      strcat(strcat(strcat(ship_menu_data[name_index], place_prefix), " "), SHIP_NAME[i]);

      char ship_symbol_upper = toupper(SHIP_NAME[i][0]);
      char ship_symbol_lower = tolower(ship_symbol_upper);
      ship_menu_data[symbol_index][0] = ship_symbol_upper;
      ship_menu_data[symbol_index][1] = ' ';
      ship_menu_data[symbol_index][2] = ship_symbol_lower;
      ship_menu_data[symbol_index][3] = '\0';

      //ship_menu_data[length_index] = SHIP_LENGTH[i];
   }

   Menu_Meta_t meta;
   Menu_Meta_Init( &meta, &ship_menu );

   Ship_Menu_Option_t choice = MENU_OPTION_SHIP_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      //clrscr();
      //Battleships_UI_Print_Grid(); // TODO
      Battleships_UI_Print_Menu(&ship_menu, &meta);
      read_success = Battleships_UI_Read_Menu(&ship_menu, &meta, (uint*) &choice);
   }
   return choice;
}

void Battleships_UI_Print_Menu(Menu_t *menu, Menu_Meta_t *meta)
{
#ifdef DEBUG
   //puts("Battleships_UI_Print_Menu");
#endif
   puts("");
   printf("%*s%s\n", meta->column_width_index, "", menu->title);
   printf("%*s%s", meta->column_width_index, "", "#");
   for (uint i=0; i<menu->num_headers; i++)
   {
      printf(" %s", menu->headers[i]);
   }
   printf("\n");
   for (uint row=0; row <menu->num_options; row++)
   {
      printf("%*s%d", meta->column_width_index, "", row);
      for (uint col=0; col<menu->num_headers; col++)
      {
         uint column_width = (col == 0) ? 1 : (meta->column_width_data[col] - meta->option_width_data[row*menu->num_headers+(col-1)] + 1) ;
         printf("%*s%s", column_width, "", menu->options[row*menu->num_headers+col]);
      }
      printf("\n");
   }
}

bool Battleships_UI_Read_Menu(Menu_t *menu, Menu_Meta_t *meta, uint *choice)
{
#ifdef DEBUG
   puts("Battleships_UI_Read_Menu");
#endif
   size_t chosen_option_len = meta->column_width_index + 1;
   String_t chosen_option_str = malloc( (chosen_option_len) * sizeof(*chosen_option_str) );
   uint chosen_option = menu->num_options;
   int retries = 0;
   bool parse_success = false;
   while (!parse_success && retries < MAX_READ_RETRIES)
   {
#ifdef DEBUG
      printf("[%d]Enter option: ", retries);
#else
      printf("Enter option: ");
#endif
      getline(&chosen_option_str, &chosen_option_len, stdin); // includes newline
      parse_success = ParseUnsignedLong(chosen_option_str, (unsigned long*) &chosen_option);
      if (chosen_option >= menu->num_options) parse_success = false;
      if (!parse_success) retries++;
#ifdef DEBUG
      //printf("input='%s'\n", chosen_option_str);
      //printf("parse=%s\n", parse_success ? "true" : "false");
      //printf("uint=%u\n", chosen_option);
#endif
   }
   free(chosen_option_str);
   *choice = chosen_option;
   return !(retries == MAX_READ_RETRIES);
}
