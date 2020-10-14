/*
 * ui.h
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIP_UI_H_
#define BATTLESHIP_UI_H_

#include "scoreboard.h"
#include "menu.h"
#include "player.h"

#include <stdio.h>

#define NUM_MAIN_MENU_OPTIONS 5 // must match enum below
#define NUM_PLACE_MENU_OPTIONS 4 // must match enum below
#define NUM_SHIP_MENU_OPTIONS 2 // must match enum below

#define SIZE_STATE_STR 1 // not including null terminator

#define STR_STATE_BLANK " "
#define STR_STATE_HIT   "*"
#define STR_STATE_MISS  "."

#define STR_GRID_SIDE_V "|"
#define STR_GRID_SIDE_H "="
#define STR_GRID_CORNER "+"
#define SIZE_GRID_SPACE 1

#define STR_DEF "DEFENSE"
#define STR_OFF "OFFENSE"

typedef enum
{
   MENU_OPTION_MAIN_RETURN,
   MENU_OPTION_MAIN_PLACE,
   MENU_OPTION_MAIN_BEGIN,
   MENU_OPTION_MAIN_WATCH,
   MENU_OPTION_MAIN_ABOUT
} Main_Menu_Option_t;

typedef enum
{
   MENU_OPTION_PLACE_RETURN,
   MENU_OPTION_PLACE_HELP,
   MENU_OPTION_PLACE_AUTO,
   MENU_OPTION_PLACE_MANUAL
} Place_Menu_Option_t;

typedef enum
{
   MENU_OPTION_SHIP_RETURN,
   MENU_OPTION_SHIP_PLACE
} Ship_Menu_Option_t;

typedef struct
{
   Ship_Menu_Option_t   option;
   Ship_Type_t          type;
} Ship_Menu_Choice_t;


void BattleShip_UI_Init();

void BattleShip_UI_Print_Logo();
void BattleShip_UI_Print_Defense_Grid(const Grid_State_t *defense);
//void BattleShip_UI_Print_Offense_Grid(const Hit_State_t *offense);
//void BattleShip_UI_Print_Both_Grids(const Grid_State_t *defense, const Hit_State_t *offense);

Main_Menu_Option_t BattleShip_UI_Main_Menu(String_t message);
Place_Menu_Option_t BattleShip_UI_Place_Menu();
Ship_Menu_Choice_t BattleShip_UI_Ship_Menu(const Grid_State_t *defense);
void BattleShip_UI_Print_Place_Help();
void BattleShip_UI_Ship_Auto();

void BattleShip_UI_Print_Menu(Menu_t *menu);
void BattleShip_UI_Print_Ship_Menu(Menu_t *menu);
bool BattleShip_UI_Read_Menu(Menu_t *menu, uint *choice);
bool BattleShip_UI_Read_Ship_Location_Heading(Coord_t *location, Heading_t *heading);

//void BattleShip_UI_Print_Scoreboard(Scoreboard_t *scoreboard);

#endif /* BATTLESHIP_UI_H_ */
