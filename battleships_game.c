/*
 * battleships.c
 *
 *  Created on: 25 Jun 2018
 *      Author: pejdavies1606
 *
 *  Last Fortran build: Dec 2017
 *
 *    References
 *    1. ASCII font 'cybermedium' http://www.topster.de/text-to-ascii/cybermedium.html
 */

// add limited number of shots, regain ammo when an enemy's ship is sunk, and lose ammo when one of your ships is sunk, equal to the length of that ship multiplied by a scale factor
// replace symbols for blank space, miss and hit: "-" -> " ", "M" -> ".", "H"-> "*"

#include "battleships_game.h"
#include "window.h"
#include "battleships_ui.h"

#include <string.h>

#define POS_WINDOW_X  10
#define POS_WINDOW_Y  10
#define SIZE_WINDOW_W 640
#define SIZE_WINDOW_H 640

//static int ans_n1, ans_n2, e1, e2, c, i, r, ax, ay1, ay2, hit_ship = 0;
//static char ans[3], ans_char, char_in[32], char_one, char_temp[32], tag_name[32], tag_value[32], msg[64];
//static _Bool sunk, two_ai = false, configExist;
//static _Bool p1sunk[NUM_SHIPS], p2sunk[NUM_SHIPS];

static Game_t game;

void Battleships_Game_Init()
{
#ifdef DEBUG
   puts("Battleships_Game_Init");
#endif

   Window_Init(POS_WINDOW_X, POS_WINDOW_Y, SIZE_WINDOW_W, SIZE_WINDOW_H);
   Rng_Init();

   Scoreboard_Init(&game.scoreboard_hit_score,
                   SCOREBOARD_HITS_TITLE,
                   NUM_PLAYERS,
                   SCOREBOARD_HITS_SCORE_WIDTH,
                   SCOREBOARD_HITS_SCORE_WIDTH);

   for (uint i = 0; i < NUM_PLAYERS; i++)
   {
      game.players[i] = Player_Init();
   }

}

void Process_Place_Menu(Place_Menu_Option_t choice)
{
   switch(choice)
   {
      case MENU_OPTION_PLACE_RETURN:
         return;
      case MENU_OPTION_PLACE_HELP:
         //Battleships_UI_Print_Place_Help(); // TODO
         break;
      case MENU_OPTION_PLACE_AUTO:
         //Battleships_UI_Ship_Auto(); // TODO
         break;
      case MENU_OPTION_PLACE_MANUAL:
         Battleships_UI_Ship_Menu();
   }
}
void Process_Main_Menu(Main_Menu_Option_t choice)
{
#ifdef DEBUG
   puts("Process_Main_Menu");
   printf("choice=%d\n", choice);
#endif
   switch(choice)
   {
      case MENU_OPTION_MAIN_RETURN:
         return;
      case MENU_OPTION_MAIN_PLACE:
         Process_Place_Menu(Battleships_UI_Place_Menu());
         break;
      case MENU_OPTION_MAIN_BEGIN:
         break;
      case MENU_OPTION_MAIN_WATCH:
         break;
      case MENU_OPTION_MAIN_ABOUT:
         break;
   }
}


void Battleships_Game_Start()
{
   //while(1) // menu loop
   //{
      Process_Main_Menu(Battleships_UI_Main_Menu(""));
   //}
}

// set ship data (x,y,z) such that all ships are on the grid and no ships overlap
// begin with longest ship, and fit others around existing ships
/*void Battleships_Set_Random_Ships()
{
   // TODO
}*/

// Identify which ship was hit and determine if it has been sunk.
/*void Battleships_Identify_Ship()
{
   // TODO
}*/

// state machine
/*void Battleships_Ai()
{
   // TODO
}*/

// TODO define these
/*static void Battleships_Logo_Print();
static void Battleships_Grid_Print_2();
static void Battleships_Grid_Print_1();
static void Battleships_Set_Ship();
static void Battleships_Clear_Ship();
static void Battleships_Enter_Ship_Data();
static void Battleships_Check_Ship_Grid();
static void Battleships_Check_Ship_Collision();
static void Battleships_Target_Print();
static void Strip_Spaces(); // for config read
 */
