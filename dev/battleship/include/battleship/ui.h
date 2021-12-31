/*
 * ui.h
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_UI_H_
#define BATTLESHIP_UI_H_

#include "battleship/scoreboard.h"
#include "battleship/menu.h"
#include "battleship/player.h"

#define NUM_LOGO_ROWS 21

#define NUM_MAIN_MENU_OPTIONS 5 // must match enum below
#define NUM_PLACE_MENU_OPTIONS 4 // must match enum below
#define NUM_SHIP_MENU_OPTIONS 2 // must match enum below

#define SIZE_CELL_STR 1 // not including null terminator

#define STR_GRID_SIDE_V "|"
#define STR_GRID_SIDE_H "="
#define STR_GRID_CORNER "+"
#define SIZE_GRID_SPACE 1

#define STR_TITLE_DEFENSE "DEFENSE"
#define STR_TITLE_OFFENSE "OFFENSE"
#define SIZE_TITLE_STR 7

#define MAX_READ_RETRIES 3

#define MAX_BUFFER_SIZE 80

typedef enum
{
   MENU_OPTION_MAIN_RETURN,
   MENU_OPTION_MAIN_PLACE,
   MENU_OPTION_MAIN_BEGIN,
   MENU_OPTION_MAIN_WATCH,
   MENU_OPTION_MAIN_ABOUT
} Main_Menu_Option_e;

typedef enum
{
   MENU_OPTION_PLACE_RETURN,
   MENU_OPTION_PLACE_HELP,
   MENU_OPTION_PLACE_AUTO,
   MENU_OPTION_PLACE_MANUAL
} Place_Menu_Option_e;

typedef enum
{
   MENU_OPTION_SHIP_RETURN,
   MENU_OPTION_SHIP_PLACE
} Ship_Menu_Option_e;

typedef struct
{
   Ship_Menu_Option_e   option;
   ShipType_e          type;
} Ship_Menu_Choice_t;

char const * BattleShip_UI_Get_Logo(int row);
Grid_Meta_t * BattleShip_UI_Get_Grid_Meta(void);

bool BattleShip_UI_Init();

void BattleShip_UI_Clear_Screen(void);
bool BattleShip_UI_Print_Message(String_t message);
void BattleShip_UI_Print_Logo();
bool BattleShip_UI_Print_Grid(Grid_t * const grid, GridHit_e * const hits);

Main_Menu_Option_e BattleShip_UI_Main_Menu(String_t message);
Place_Menu_Option_e BattleShip_UI_Place_Menu(Grid_t * const grid);
Ship_Menu_Choice_t BattleShip_UI_Ship_Menu_Manual(Grid_t * const grid);

bool BattleShip_UI_Print_Menu(Menu_t *menu);
bool BattleShip_UI_Read_Menu(Menu_t *menu, uint *choice);
bool BattleShip_UI_Read_Ship_Location_Heading(Coord_t *location, Heading_e *heading);

void BattleShip_UI_Game_Screen(Grid_t * const grid, GridHit_e * const hits);

//void BattleShip_UI_Print_Scoreboard(Scoreboard_t *scoreboard);

#endif /* BATTLESHIP_UI_H_ */
