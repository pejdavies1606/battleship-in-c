/*
 * ui.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "battleship/ship.h"
#include "battleship/grid.h"
#include "battleship/util.h"
#include "battleship/input.h"
#include "battleship/ui.h"

#ifndef NDEBUG
#define clrscr() puts("\n-----clrscr-----")
#else
// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
#if defined(MSDOS) && MSDOS == 1
// Borland-style CONIO implementation for MinGW/Dev-C++ (http://conio.sourceforge.net/)
#include "conio21/conio2.h"
#else
#define clrscr() printf("\e[1;1H\e[2J")
#endif
#endif

#define MAX_READ_RETRIES 3

#define NUM_MAIN_MENU_HEADERS 1
#define NUM_PLACE_MENU_HEADERS 1
#define NUM_SHIP_MENU_HEADERS 2

static int BattleShip_UI_Read(
   String_t prompt,
   size_t option_len,
   uint option_max,
   uint *choice,
   InputParser_t InputParser);

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
      uint num_options = NUM_SHIPS + 1;
      String_t *ship_menu_data = malloc(sizeof(String_t) * num_options * NUM_SHIP_MENU_HEADERS);
      String_t return_str = ship_menu_options[MENU_OPTION_SHIP_RETURN];
      String_t place_prefix = ship_menu_options[MENU_OPTION_SHIP_PLACE];
      uint length_len = (uint)CalcNumWidth(CalcMax((int *)Ship_Get_Length_Array(), NUM_SHIPS)) + 1;

      for (uint i = 0; i < num_options; i++)
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
            const Ship_Info_t *ship_info = &shipTable[i-1];
            if (ship_info)
            {
               String_t ship_name = ship_info->name;
               uint ship_length = ship_info->length;
               size_t name_len = strlen(place_prefix) + 1 + strlen(ship_name) + 1;
               ship_menu_data[name_index] = malloc(name_len);
               ship_menu_data[length_index] = malloc(length_len);
               snprintf(ship_menu_data[name_index], name_len, "%s %s", place_prefix, ship_name);
               snprintf(ship_menu_data[length_index], length_len, "%u", ship_length);
            }
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
   printf("%s\n", __FUNCTION__);
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

// TODO replace GRID_SIZE with grid params
Status_t BattleShip_UI_Print_Grid_Defense(const Grid_t *grid)
{
   Grid_State_t *defense = grid->defense;
   if (!defense)
   {
      return STATUS_ERROR;
   }
   char ship_str[SIZE_STATE_STR + 1];
   char state_str[SIZE_STATE_STR + 1];
   memset(ship_str, 0, sizeof(ship_str));
   memset(state_str, 0, sizeof(state_str));

   printf("%*s%s\n",
      (int)((GRID_SIZE/2)*(grid_meta.col_width + SIZE_GRID_SPACE)), "",
      STR_DEF);

   for (int row = -1; row < GRID_SIZE+1; row++)
   {
      if (row == -1)
      {
         // style choice: SIZE_GRID_SPACE instead of corner_len
         printf("%*s%.*s", (int)(grid_meta.col_width - 1), "",
            SIZE_GRID_SPACE, grid_meta.corner_str);
         for (uint col = 0; col < GRID_SIZE; col++)
         {
            printf("%*s%-*c", SIZE_GRID_SPACE, "", 
               (int)grid_meta.col_width, col + 65); // ASCII
         }
         printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_CORNER);
      }
      else if(row == GRID_SIZE)
      {
         // style choice: SIZE_GRID_SPACE instead of corner_len
         printf("%*s%.*s", (int)(grid_meta.col_width - 1), "",
            SIZE_GRID_SPACE, grid_meta.corner_str);
         printf("%*s%.*s", SIZE_GRID_SPACE, "",
            (int)grid_meta.side_len, grid_meta.side_str);
         printf("%*s%s\n", SIZE_GRID_SPACE, "",
            STR_GRID_CORNER);
      }
      else
      {
         printf("%*u", (int)grid_meta.col_width, row + 1);
         for (int col = 0; col < GRID_SIZE; col++)
         {
            Grid_State_t p = defense[row*GRID_SIZE + col];
            ShipType2Str(p.ship_type, ship_str, sizeof(ship_str));
            HitState2Str(p.hit_state, state_str, sizeof(state_str));
            printf("%*s%s%s", SIZE_GRID_SPACE, "", ship_str, state_str);
         }
         printf("%*s%s\n", SIZE_GRID_SPACE, "", STR_GRID_SIDE_V);
      }
   }
   return STATUS_OK;
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
   printf("%s\n", __FUNCTION__);
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
   }
   return choice;
}

Place_Menu_Option_t BattleShip_UI_Place_Menu(const Grid_t *grid)
{
#ifndef NDEBUG
   printf("%s\n", __FUNCTION__);
#endif
   Place_Menu_Option_t choice = MENU_OPTION_PLACE_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      BattleShip_UI_Print_Grid_Defense(grid);
      BattleShip_UI_Print_Menu(&place_menu);
      read_success = BattleShip_UI_Read_Menu(&place_menu, (uint*) &choice);
   }
   return choice;
}

Ship_Menu_Choice_t BattleShip_UI_Ship_Menu_Manual(const Grid_t *grid)
{
#ifndef NDEBUG
   printf("%s\n", __FUNCTION__);
#endif
   Ship_Menu_Option_t option = MENU_OPTION_SHIP_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      clrscr();
      BattleShip_UI_Print_Grid_Defense(grid);
      BattleShip_UI_Print_Menu(&ship_menu);
      read_success = BattleShip_UI_Read_Menu(&ship_menu, (uint*) &option);
   }
   return (Ship_Menu_Choice_t)
   {
      (option == MENU_OPTION_SHIP_RETURN) ? option : MENU_OPTION_SHIP_PLACE,
      (Ship_Type_t) (option - 1)
   };
}

void BattleShip_UI_Print_Menu(Menu_t *menu)
{
#ifndef NDEBUG
   printf("%s\n", __FUNCTION__);
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
   printf("%s\n", __FUNCTION__);
#endif
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
#ifndef NDEBUG
   printf("%s\n", __FUNCTION__);
#endif
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
