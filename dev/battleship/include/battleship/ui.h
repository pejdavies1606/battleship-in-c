/*
 * ui.h
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_UI_H_
#define BATTLESHIP_UI_H_

#include "battleship/scoreboard.h"
#include "battleship/form.h"
#include "battleship/menu.h"
#include "battleship/player.h"
#include "battleship/game.h"
#include "battleship/grid.h"
#include "battleship/line.h"

#define NUM_LOGO_ROWS 21

#define NUM_MAIN_MENU_OPTIONS 5 // must match enum below
#define NUM_PLACE_MENU_OPTIONS 4 // must match enum below
#define NUM_SHIP_MENU_OPTIONS 2 // must match enum below
#define NUM_SHIP_MENU_OPTIONS_FULL (1 + NUM_SHIPS)

#define MAX_READ_RETRIES 3

typedef enum MainMenuOption
{
   MENU_OPTION_MAIN_RETURN,
   MENU_OPTION_MAIN_PLACE,
   MENU_OPTION_MAIN_BEGIN,
   MENU_OPTION_MAIN_WATCH,
   MENU_OPTION_MAIN_ABOUT
} MainMenuOption_e;

typedef enum PlaceMenuOption
{
   MENU_OPTION_PLACE_RETURN,
   MENU_OPTION_PLACE_HELP,
   MENU_OPTION_PLACE_AUTO,
   MENU_OPTION_PLACE_MANUAL
} PlaceMenuOption_e;

typedef enum ShipMenuOption
{
   MENU_OPTION_SHIP_RETURN,
   MENU_OPTION_SHIP_PLACE
} ShipMenuOption_e;

typedef struct ShipMenuChoice
{
   ShipMenuOption_e option;
   ShipType_e type;
} ShipMenuChoice_t;

char const * BattleShipUI_GetLogo(int row);

bool BattleShipUI_Init();
void BattleShipUI_Destroy();

void BattleShipUI_ClearScreen();
bool BattleShipUI_PrintMessage(
   char const *const message);
void BattleShipUI_PrintLogo();
bool BattleShipUI_PrintGrid(
   Grid_t const *const def_grid,
   Grid_t const *const off_grid);

MainMenuOption_e BattleShipUI_MainMenu(
   char const *const message);
PlaceMenuOption_e BattleShipUI_PlaceMenu(
   Grid_t *const grid);
ShipMenuChoice_t BattleShipUI_ShipMenuManual(
   Grid_t *const grid);

bool BattleShipUI_PrintMenu(Menu_t *menu);
bool BattleShipUI_ReadMenu(Menu_t *menu, uint *choice);

bool BattleShipUI_CoordForm(Coord_t *location);
bool BattleShipUI_HeadingForm(Heading_e *heading);
bool BattleShipUI_ReadForm(Form_t *form, InputVal_t *val);

bool BattleShipUI_GameScreen(
   Game_t const *const game,
   Line_t const *const message);

//void BattleShip_UI_Print_Scoreboard(Scoreboard_t *scoreboard);

#endif /* BATTLESHIP_UI_H_ */
