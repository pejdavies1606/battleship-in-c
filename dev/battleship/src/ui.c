/*
 * ui.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include <string.h>
#include <ctype.h>
#include "battleship/ship.h"
#include "battleship/grid.h"
#include "battleship/util.h"
#include "battleship/input.h"
#include "battleship/ui.h"

#define NUM_MAIN_MENU_HEADERS 1
#define NUM_PLACE_MENU_HEADERS 1
#define NUM_SHIP_MENU_HEADERS 2

// ASCII font 'cybermedium' http://www.topster.de/text-to-ascii/cybermedium.html
char const *LOGO[NUM_LOGO_ROWS] =
{
    "                       |                           ",
    "                       |                           ",
    "                 .  -  +  -  .                     ",
    "              +'       |       '+                  ",
    "            +          |          +                ",
    " ___  ____+___ ___ _   |____ ____ _ +_ _ ___  ____ ",
    " |__] |__|  |   |  |   ||___ [__  |__| | |__] [__  ",
    " |__] | /|  |   |  |___||___ ___] |  |\\| |    ___] ",
    "       '               |               '           ",
    "       |             _ |_  _           |           ",
    "   ----+-------------|-+|\\ |-----------+----       ",
    "       |             | || \\|           |           ",
    "       .            _______            .           ",
    "       \\            |  |               /           ",
    "        \\           |  |              /            ",
    "          +         |__|___         +              ",
    "            +          |          +                ",
    "              +.       |       .+                  ",
    "                 '  -  +  -  '                     ",
    "                       |                           ",
    "                       |                           "
};

static char * MAIN_MENU_HEADERS[NUM_MAIN_MENU_HEADERS] =
{
   "Option"
};

static char * MAIN_MENU_OPTIONS[NUM_MAIN_MENU_OPTIONS] =
{
   "Quit",
   "Place your ships",
   "Begin battle!",
   "Watch an automated battle",
   "About"
};

static Menu_t MAIN_MENU =
{
   .title = "Main Menu",
   .num_headers = NUM_MAIN_MENU_HEADERS,
   .headers = MAIN_MENU_HEADERS,
   .num_options = NUM_MAIN_MENU_OPTIONS,
   .options = MAIN_MENU_OPTIONS
};

static char * PLACE_MENU_HEADERS[NUM_PLACE_MENU_HEADERS] =
{
   "Option",
};

static char * PLACE_MENU_OPTIONS[NUM_PLACE_MENU_OPTIONS] =
{
   "Return",
   "Help",
   "Automatic",
   "Manual"
};

static Menu_t PLACE_MENU =
{
   .title = "Ship Placement Menu",
   .num_headers = NUM_PLACE_MENU_HEADERS,
   .headers = PLACE_MENU_HEADERS,
   .num_options = NUM_PLACE_MENU_OPTIONS,
   .options = PLACE_MENU_OPTIONS
};

static char * SHIP_MENU_HEADERS[NUM_SHIP_MENU_HEADERS] =
{
   "Option",
   "Length"
};

// stored column-wise, printed row-wise
static char * SHIP_MENU_OPTIONS[NUM_SHIP_MENU_OPTIONS*NUM_SHIP_MENU_HEADERS] =
{
   "Return", "Place",
   "L0", "L1",
};

static Menu_t SHIP_MENU =
{
   .title = "Ships",
   .num_headers = NUM_SHIP_MENU_HEADERS,
   .headers = SHIP_MENU_HEADERS,
};

bool BattleShipUI_Init(void)
{
   bool result = false;
   if (Menu_InitMeta(&MAIN_MENU) &&
       Menu_InitMeta(&PLACE_MENU))
   {
      // complete ship menu at runtime
      uint num_options = NUM_SHIPS + 1;
      size_t ship_menu_size = sizeof(char *) * num_options * NUM_SHIP_MENU_HEADERS;
      char * *ship_menu_data = malloc(ship_menu_size);
      if (ship_menu_data)
      {
         memset(ship_menu_data, 0, ship_menu_size);
         char * return_str = SHIP_MENU_OPTIONS[MENU_OPTION_SHIP_RETURN];
         char * place_prefix = SHIP_MENU_OPTIONS[MENU_OPTION_SHIP_PLACE];
         uint length_len = 0;
         uint *ship_length = Ship_GetLengths();
         if (ship_length)
         {
            length_len = (uint)CalcNumWidth(CalcMax((int *)ship_length, NUM_SHIPS)) + 1;
            free(ship_length);
            ship_length = NULL;
         }
         result = true;
         for (uint i = 0; i < num_options; i++)
         {
            uint name_index = i;
            uint length_index = i + num_options;
            if (i == 0)
            {
               size_t return_len = strlen(return_str) + 1;
               ship_menu_data[name_index] = malloc(return_len);
               if (ship_menu_data[name_index])
               {
                  snprintf(ship_menu_data[name_index], return_len, "%s", return_str);
                  ship_menu_data[length_index] = ""; // blank entry
               }
               else
               {
                  result = false;
                  break;
               }
            }
            else
            {
               Ship_Info_t const * ship_info = &SHIP_TABLE[i - 1];
               if (ship_info)
               {
                  char * ship_name = ship_info->name;
                  uint ship_length = ship_info->length;
                  size_t name_len = strlen(place_prefix) + 1 + strlen(ship_name) + 1;
                  ship_menu_data[name_index] = malloc(name_len);
                  ship_menu_data[length_index] = malloc(length_len);
                  if (ship_menu_data[name_index] && ship_menu_data[length_index])
                  {
                     snprintf(ship_menu_data[name_index], name_len, "%s %s", place_prefix, ship_name);
                     snprintf(ship_menu_data[length_index], length_len, "%u", ship_length);
                  }
                  else
                  {
                     result = false;
                     break;
                  }
               }
            }
         }
         if (result)
         {
            // redefine template options table in menu with new, completed options table
            SHIP_MENU.num_options = num_options;
            SHIP_MENU.options = ship_menu_data;
            result = Menu_InitMeta(&SHIP_MENU);
         }
      }
   }
   return result;
}

char const * BattleShipUI_GetLogo(int row)
{
   char const * result = NULL;
   if (row >= 0 && row < NUM_LOGO_ROWS)
   {
      result = LOGO[row];
   }
   return result;
}

MainMenuOption_e BattleShipUI_MainMenu(char * message)
{
   MainMenuOption_e choice = MENU_OPTION_MAIN_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShipUI_ClearScreen();
      BattleShipUI_PrintLogo();
      BattleShipUI_PrintMessage(message);
      BattleShipUI_PrintMenu(&MAIN_MENU);
      read_success = BattleShipUI_ReadMenu(&MAIN_MENU, (uint*) &choice);
   }
   return choice;
}

PlaceMenuOption_e BattleShipUI_PlaceMenu(Grid_t * const grid)
{
   PlaceMenuOption_e choice = MENU_OPTION_PLACE_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShipUI_ClearScreen();
      BattleShipUI_PrintGrid(grid, NULL);
      BattleShipUI_PrintMenu(&PLACE_MENU);
      read_success = BattleShipUI_ReadMenu(&PLACE_MENU, (uint*) &choice);
   }
   return choice;
}

Ship_Menu_Choice_t BattleShipUI_ShipMenuManual(Grid_t * const grid)
{
   Ship_Menu_Option_e option = MENU_OPTION_SHIP_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShipUI_ClearScreen();
      BattleShipUI_PrintGrid(grid, NULL);
      BattleShipUI_PrintMenu(&SHIP_MENU);
      read_success = BattleShipUI_ReadMenu(&SHIP_MENU, (uint*) &option);
   }
   return (Ship_Menu_Choice_t)
   {
      (option == MENU_OPTION_SHIP_RETURN) ? option : MENU_OPTION_SHIP_PLACE,
      (ShipType_e) (option - 1)
   };
}

void BattleShipUI_GameScreen(
   Grid_t * const grid,
   GridHit_e * const hits)
{
   BattleShipUI_ClearScreen();
   BattleShipUI_PrintGrid(grid, hits);
   /* screen
    *    defense + offense grids
    *    scoreboards
    *    round counter
    *    player turns
    *       hit/miss
    *       ship sunk
    *       all ships sunk
    */
}
