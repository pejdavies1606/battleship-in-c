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
#include "battleship/form.h"
#include "battleship/ui.h"
#include "battleship/window.h"

#define NUM_MAIN_MENU_HEADERS    1
#define NUM_PLACE_MENU_HEADERS   1
#define NUM_SHIP_MENU_HEADERS    2
#define NUM_COORD_FORM_FIELDS    1
#define NUM_HEADING_FORM_FIELDS  1

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

#define STR_SHIP_MENU_RETURN "Return"
#define STR_SHIP_MENU_PLACE "Place"

static char * SHIP_MENU_OPTIONS[NUM_SHIP_MENU_OPTIONS_FULL * NUM_SHIP_MENU_HEADERS] = {0};

static Menu_t SHIP_MENU =
{
   .title = "Ships",
   .num_headers = NUM_SHIP_MENU_HEADERS,
   .headers = SHIP_MENU_HEADERS,
   .num_options = NUM_SHIP_MENU_OPTIONS_FULL,
   .options = SHIP_MENU_OPTIONS,
};

static InputData_t COORD_INPUT_DATA =
{
   .type = INPUT_COORD,
   .max.loc.col = MAX_COORD_COL,
   .max.loc.row = MAX_COORD_ROW,
};

static Field_t COORD_FORM_FIELDS[NUM_COORD_FORM_FIELDS] =
{
   {
      .prompt = "<A1-J10>",
      .len = LEN_COORD,
      .data = &COORD_INPUT_DATA
   },
};

static Form_t COORD_FORM =
{
   .title = "Location",
   .num_fields = NUM_COORD_FORM_FIELDS,
   .fields = COORD_FORM_FIELDS,
};

static InputData_t HEADING_INPUT_DATA =
{
   .type = INPUT_HEADING,
};

static Field_t HEADING_FORM_FIELDS[NUM_HEADING_FORM_FIELDS] =
{
   {
      .prompt = "<N|E|S|W>",
      .len = LEN_HEADING,
      .data = &HEADING_INPUT_DATA
   },
};

static Form_t HEADING_FORM =
{
   .title = "Heading",
   .num_fields = NUM_HEADING_FORM_FIELDS,
   .fields = HEADING_FORM_FIELDS,
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
   // complete ship menu at runtime
   size_t ret_len = strnlen(
      STR_SHIP_MENU_PLACE,
      SIZE_MENU_COL_MAX);
   if (ret_len > 0 && ret_len < SIZE_MENU_COL_MAX)
   {
      ret_len++;
      SHIP_MENU_OPTIONS[0] = malloc(
         ret_len * sizeof(char));
      if (SHIP_MENU_OPTIONS[0])
      {
         result = (strncpy(
            SHIP_MENU_OPTIONS[0],
            STR_SHIP_MENU_RETURN,
            ret_len));
      }
   }
   if (result)
   {
      const char *place_prefix = STR_SHIP_MENU_PLACE;
      for (uint i = 0; i < NUM_SHIPS; i++)
      {
         uint name_index = i + 1;
         uint length_index = name_index + NUM_SHIP_MENU_OPTIONS_FULL;
         Ship_Info_t const *ship_info = &SHIP_TABLE[i];
         if (ship_info)
         {
            size_t place_len = strnlen(
               place_prefix,
               SIZE_MENU_COL_MAX);
            size_t ship_len = strnlen(
               ship_info->name,
               SIZE_MENU_COL_MAX);
            if (place_len > 0 && place_len < SIZE_MENU_COL_MAX &&
                ship_len > 0 && ship_len < SIZE_MENU_COL_MAX)
            {
               size_t name_len = place_len + 1 + ship_len + 1;
               size_t length_len = LEN_SHIPS + 1;
               if (name_len < SIZE_MENU_COL_MAX)
               {
                  SHIP_MENU_OPTIONS[name_index] = malloc(
                     name_len * sizeof(char));
                  SHIP_MENU_OPTIONS[length_index] = malloc(
                     length_len * sizeof(char));
                  if (SHIP_MENU_OPTIONS[name_index] &&
                      SHIP_MENU_OPTIONS[length_index])
                  {
                     snprintf(
                         SHIP_MENU_OPTIONS[name_index],
                         name_len,
                         "%s %s",
                         place_prefix,
                         ship_info->name);
                     snprintf(
                         SHIP_MENU_OPTIONS[length_index],
                         length_len,
                         "%u",
                         ship_info->length);
                  }
                  else
                  {
                     result = false;
                     break;
                  }
               }
            }
         }
      }
   }
   if (result)
   {
      bool res_main = Menu_InitData(&MAIN_MENU);
      bool res_place = Menu_InitData(&PLACE_MENU);
      bool res_ship = Menu_InitData(&SHIP_MENU);
      result = (res_main && res_place && res_ship);
   }
   if (result)
   {
      bool res_loc = Form_InitData(&COORD_FORM);
      bool res_hdg = Form_InitData(&HEADING_FORM);
      result = res_loc && res_hdg;
   }
   if (!result)
   {
      BattleShipUI_Destroy();
   }
   return result;
}

void BattleShipUI_Destroy(void)
{
   Menu_DestroyData(&MAIN_MENU);
   Menu_DestroyData(&PLACE_MENU);
   Menu_DestroyData(&SHIP_MENU);

   Form_DestroyData(&COORD_FORM);
   Form_DestroyData(&HEADING_FORM);

   for (uint i = 0; i < SHIP_MENU.num_options; i++)
   {
      uint name_index = i;
      uint length_index = i + SHIP_MENU.num_options;
      Destroy((void **)&SHIP_MENU.options[name_index]);
      Destroy((void **)&SHIP_MENU.options[length_index]);
   }
}

MainMenuOption_e BattleShipUI_MainMenu(
   char const * const message)
{
   MainMenuOption_e choice = MENU_OPTION_MAIN_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShipUI_ClearScreen();
      BattleShipUI_PrintLogo();
      bool messageResult = BattleShipUI_PrintMessage(message);
      bool menuResult = BattleShipUI_PrintMenu(&MAIN_MENU);
      if (!(messageResult && menuResult))
      {
         break;
      }
      read_success = BattleShipUI_ReadMenu(&MAIN_MENU, (uint*) &choice);
   }
   return choice;
}

PlaceMenuOption_e BattleShipUI_PlaceMenu(
   Grid_t * const grid)
{
   PlaceMenuOption_e choice = MENU_OPTION_PLACE_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShipUI_ClearScreen();
      bool gridResult = BattleShipUI_PrintGrid(grid, NULL);
      bool menuResult = BattleShipUI_PrintMenu(&PLACE_MENU);
      if (!(gridResult && menuResult))
      {
         break;
      }
      read_success = BattleShipUI_ReadMenu(&PLACE_MENU, (uint*) &choice);
   }
   return choice;
}

ShipMenuChoice_t BattleShipUI_ShipMenuManual(
   Grid_t * const grid)
{
   ShipMenuChoice_t result = {0};
   uint choice = 0;
   bool read_success = false;
   while(!read_success)
   {
      BattleShipUI_ClearScreen();
      bool gridResult = BattleShipUI_PrintGrid(grid, NULL);
      bool menuResult = BattleShipUI_PrintMenu(&SHIP_MENU);
      if (!(gridResult && menuResult))
      {
         break;
      }
      read_success = BattleShipUI_ReadMenu(&SHIP_MENU, &choice);
   }
   if (choice > 0)
   {
      result.option = MENU_OPTION_SHIP_PLACE;
      result.type = (ShipType_e)(choice);
   }
   return result;
}

bool BattleShipUI_CoordForm(Coord_t *location)
{
   bool result = false;
   if (location)
   {
      InputVal_t val = {0};
      result = BattleShipUI_ReadForm(&COORD_FORM, &val);
      *location = val.loc;
   }
   return result;
}

bool BattleShipUI_HeadingForm(Heading_e *heading)
{
   bool result = false;
   if (heading)
   {
      InputVal_t val = {0};
      result = BattleShipUI_ReadForm(&HEADING_FORM, &val);
      *heading = val.hdg;
   }
   return result;
}

bool BattleShipUI_GameScreen(
   Game_t const * const game,
   Line_t const *const message)
{
   bool result = false;
   if (game)
   {
      BattleShipUI_ClearScreen();
      result = BattleShipUI_PrintGrid(
          &game->players[0].grid,
          &game->players[1].grid);
#ifdef DEBUG
      if (result)
      {
         result = BattleShipUI_PrintGrid(
             &game->players[1].grid,
             &game->players[0].grid);
      }
#endif
      if (result && message)
      {
         result = BattleShipUI_PrintMessage(message->buffer);
      }
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
   return result;
}
