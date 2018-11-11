/*
 * battleships_print.h
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef BATTLESHIPS_UI_H_
#define BATTLESHIPS_UI_H_

#include "scoreboard.h"
#include "menu.h"

#include <stdio.h>

#define NUM_MAIN_MENU_OPTIONS 5 // must match enum below
#define NUM_PLACE_MENU_OPTIONS 4 // must match enum below
#define NUM_SHIP_MENU_OPTIONS 2 // must match enum below

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

void Battleships_UI_Print_Logo();

Main_Menu_Option_t Battleships_UI_Main_Menu(String_t message);
Place_Menu_Option_t Battleships_UI_Place_Menu();
Ship_Menu_Option_t Battleships_UI_Ship_Menu();
void Battleships_UI_Print_Place_Help();
void Battleships_UI_Ship_Auto();

void Battleships_UI_Print_Menu(Menu_t *menu, Menu_Meta_t *meta);
void Battleships_UI_Print_Ship_Menu(Menu_t *menu, Menu_Meta_t *meta);
bool Battleships_UI_Read_Menu(Menu_t *menu, Menu_Meta_t *meta, uint *choice);
void Battleships_UI_Print_Scoreboard(Scoreboard_t *scoreboard);

#endif /* BATTLESHIPS_UI_H_ */
