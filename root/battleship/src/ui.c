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

bool BattleShip_UI_Init(void)
{
   bool result = false;
   if (Menu_Meta_Init(&main_menu) &&
       Menu_Meta_Init(&place_menu))
   {
      // complete ship menu at runtime
      uint num_options = NUM_SHIPS + 1;
      size_t ship_menu_size = sizeof(String_t) * num_options * NUM_SHIP_MENU_HEADERS;
      String_t *ship_menu_data = malloc(ship_menu_size);
      if (ship_menu_data)
      {
         memset(ship_menu_data, 0, ship_menu_size);
         String_t return_str = ship_menu_options[MENU_OPTION_SHIP_RETURN];
         String_t place_prefix = ship_menu_options[MENU_OPTION_SHIP_PLACE];
         uint length_len = 0;
         uint *ship_length = Ship_Get_Length_Array();
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
               const Ship_Info_t *ship_info = &shipTable[i - 1];
               if (ship_info)
               {
                  String_t ship_name = ship_info->name;
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
            ship_menu.num_options = num_options;
            ship_menu.options = ship_menu_data;
            result = Menu_Meta_Init(&ship_menu);
         }
      }
   }
   return result;
}

Main_Menu_Option_e BattleShip_UI_Main_Menu(String_t message)
{
   Main_Menu_Option_e choice = MENU_OPTION_MAIN_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShip_UI_Clear_Screen();
      BattleShip_UI_Print_Logo();
      BattleShip_UI_Print_Message(message);
      BattleShip_UI_Print_Menu(&main_menu);
      read_success = BattleShip_UI_Read_Menu(&main_menu, (uint*) &choice);
   }
   return choice;
}

Place_Menu_Option_e BattleShip_UI_Place_Menu(const Grid_t *grid)
{
   Place_Menu_Option_e choice = MENU_OPTION_PLACE_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShip_UI_Clear_Screen();
      BattleShip_UI_Print_Grid_Defense(grid);
      BattleShip_UI_Print_Menu(&place_menu);
      read_success = BattleShip_UI_Read_Menu(&place_menu, (uint*) &choice);
   }
   return choice;
}

Ship_Menu_Choice_t BattleShip_UI_Ship_Menu_Manual(const Grid_t *grid)
{
   Ship_Menu_Option_e option = MENU_OPTION_SHIP_RETURN;
   bool read_success = false;
   while(!read_success)
   {
      BattleShip_UI_Clear_Screen();
      BattleShip_UI_Print_Grid_Defense(grid);
      BattleShip_UI_Print_Menu(&ship_menu);
      read_success = BattleShip_UI_Read_Menu(&ship_menu, (uint*) &option);
   }
   return (Ship_Menu_Choice_t)
   {
      (option == MENU_OPTION_SHIP_RETURN) ? option : MENU_OPTION_SHIP_PLACE,
      (Ship_Type_e) (option - 1)
   };
}
