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

static char * SHIP_MENU_OPTIONS[NUM_SHIP_MENU_OPTIONS*NUM_SHIP_MENU_HEADERS] =
{
   "Return",
   "Place",
};

static Menu_t SHIP_MENU =
{
   .title = "Ships",
   .num_headers = NUM_SHIP_MENU_HEADERS,
   .headers = SHIP_MENU_HEADERS,
};

char const * BattleShipUI_GetLogo(int row)
{
   char const * result = NULL;
   if (row >= 0 && row < NUM_LOGO_ROWS)
   {
      result = LOGO[row];
   }
   return result;
}

bool BattleShipUI_Init(void)
{
   bool result = false;
   if (Menu_InitMeta(&MAIN_MENU) &&
       Menu_InitMeta(&PLACE_MENU))
   {
      // complete ship menu at runtime
      size_t ship_menu_size =
          sizeof(char *) * NUM_SHIP_MENU_OPTIONS * NUM_SHIP_MENU_HEADERS;
      char **ship_menu_data = malloc(ship_menu_size);
      if (ship_menu_data)
      {
         memset(ship_menu_data, 0, ship_menu_size);
         ship_menu_data[0] = SHIP_MENU_OPTIONS[MENU_OPTION_SHIP_RETURN];
         char *place_prefix = SHIP_MENU_OPTIONS[MENU_OPTION_SHIP_PLACE];
         result = true;
         for (uint i = 0; i < NUM_SHIPS; i++)
         {
            uint name_index = i + 1;
            uint length_index = name_index + NUM_SHIP_MENU_OPTIONS;
            Ship_Info_t const *ship_info = &SHIP_TABLE[i];
            if (ship_info)
            {
               size_t name_len =
                   strlen(place_prefix) + 1 +
                   strlen(ship_info->name) + 1;
               size_t length_len = LEN_SHIPS + 1;
               ship_menu_data[name_index] = malloc(name_len);
               ship_menu_data[length_index] = malloc(length_len);
               if (
                   ship_menu_data[name_index] &&
                   ship_menu_data[length_index])
               {
                  snprintf(
                      ship_menu_data[name_index],
                      name_len,
                      "%s %s",
                      place_prefix,
                      ship_info->name);
                  snprintf(
                      ship_menu_data[length_index],
                      length_len,
                      "%u",
                      ship_info->length);
               }
               else
               {
                  result = false;
                  Destroy((void **)&ship_menu_data[name_index]);
                  Destroy((void **)&ship_menu_data[length_index]);
                  break;
               }
            }
         }
         if (result)
         {
            // redefine template options table in menu with new, completed options table
            SHIP_MENU.num_options = NUM_SHIP_MENU_OPTIONS;
            SHIP_MENU.options = ship_menu_data;
            result = Menu_InitMeta(&SHIP_MENU);
         }
      }
   }
   return result;
}

void BattleShipUI_Destroy(void)
{
   Menu_DestroyMeta(&MAIN_MENU);
   Menu_DestroyMeta(&PLACE_MENU);
   Menu_DestroyMeta(&SHIP_MENU);
   if (SHIP_MENU.options)
   {
      for (uint i = 0; i < SHIP_MENU.num_options; i++)
      {
         uint name_index = i;
         uint length_index = i + SHIP_MENU.num_options;
         Destroy((void **)&SHIP_MENU.options[name_index]);
         Destroy((void **)&SHIP_MENU.options[length_index]);
      }
      Destroy((void **)&SHIP_MENU.options);
      SHIP_MENU.num_options = 0;
   }
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

ShipMenuChoice_t BattleShipUI_ShipMenuManual(Grid_t * const grid)
{
   ShipMenuChoice_t result = {0};
   uint choice = 0;
   bool read_success = false;
   while(!read_success)
   {
      BattleShipUI_ClearScreen();
      if (BattleShipUI_PrintGrid(grid, NULL) &&
          BattleShipUI_PrintMenu(&SHIP_MENU))
      {
         read_success = BattleShipUI_ReadMenu(&SHIP_MENU, &choice);
      }
      else
      {
         break;
      }
   }
   if (choice > 0)
   {
      result.option = MENU_OPTION_SHIP_PLACE;
      result.type = (ShipType_e)(choice);
   }
   return result;
}

void BattleShipUI_GameScreen(
   Grid_t * const grid,
   GridState_e * const states)
{
   BattleShipUI_ClearScreen();
   BattleShipUI_PrintGrid(grid, states);
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
