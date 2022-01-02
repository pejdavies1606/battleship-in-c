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
} MainMenuOption_e;

typedef enum
{
   MENU_OPTION_PLACE_RETURN,
   MENU_OPTION_PLACE_HELP,
   MENU_OPTION_PLACE_AUTO,
   MENU_OPTION_PLACE_MANUAL
} PlaceMenuOption_e;

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

char const * BattleShipUI_GetLogo(int row);
Grid_Meta_t * BattleShip_UI_Get_Grid_Meta(void);

bool BattleShipUI_Init();

void BattleShipUI_ClearScreen(void);
bool BattleShipUI_PrintMessage(String_t message);
void BattleShipUI_PrintLogo();
bool BattleShipUI_PrintGrid(Grid_t * const grid, GridHit_e * const hits);

MainMenuOption_e BattleShipUI_MainMenu(String_t message);
PlaceMenuOption_e BattleShipUI_PlaceMenu(Grid_t * const grid);
Ship_Menu_Choice_t BattleShipUI_ShipMenuManual(Grid_t * const grid);

bool BattleShipUI_PrintMenu(Menu_t *menu);
bool BattleShipUI_ReadMenu(Menu_t *menu, uint *choice);
bool BattleShipUI_ReadShipCoord(Coord_t *location, Heading_e *heading);

void BattleShipUI_GameScreen(Grid_t * const grid, GridHit_e * const hits);

//void BattleShip_UI_Print_Scoreboard(Scoreboard_t *scoreboard);

#endif /* BATTLESHIP_UI_H_ */
