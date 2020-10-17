/*
 * game.c
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

#include "game.h"
#include "window.h"
#include "ui.h"
#include "rng.h"

#include <string.h>

#define POS_WINDOW_X  10
#define POS_WINDOW_Y  10
#define SIZE_WINDOW_W 640
#define SIZE_WINDOW_H 640

//static int ans_n1, ans_n2, e1, e2, c, i, r, ax, ay1, ay2, hit_ship = 0;
//static char ans[3], ans_char, char_in[32], char_one, char_temp[32], tag_name[32], tag_value[32], msg[64];
//static _Bool sunk, two_ai = false, configExist;
//static _Bool p1sunk[NUM_SHIPS], p2sunk[NUM_SHIPS];

static void Process_Ship_Menu(Player_t *player, Ship_Menu_Choice_t choice);
static void Process_Place_Menu(Player_t *player, Place_Menu_Option_t choice);
static void Process_Main_Menu(Player_t *player, Main_Menu_Option_t choice);

void BattleShip_Game_Init(Game_t *game)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   Window_Init(
      POS_WINDOW_X, POS_WINDOW_Y,
      SIZE_WINDOW_W, SIZE_WINDOW_H);
#ifndef NDEBUG
   Rng_Init(1606);
#else
   Rng_Init(0);
#endif

   if (game)
   {
      memset(game, 0, sizeof(Game_t));

      Scoreboard_Init(
         &game->scoreboard_hit_score,
         SCOREBOARD_HITS_TITLE,
         NUM_PLAYERS,
         SCOREBOARD_HITS_SCORE_WIDTH,
         SCOREBOARD_HITS_SCORE_WIDTH);

      game->players = malloc(NUM_PLAYERS * sizeof(Player_t));
      if (game->players)
      {
         for (uint i = 0; i < NUM_PLAYERS; i++)
         {
            game->players[i] = Player_Init();
         }
      }
   }
   BattleShip_UI_Init();
}

void BattleShip_Game_Start(Game_t *game)
{
   if (game)
   {
      // TODO while loop
      Process_Main_Menu(&game->players[0], BattleShip_UI_Main_Menu(""));
   }
}

static void Process_Ship_Menu(Player_t *player, Ship_Menu_Choice_t choice)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   switch(choice.option)
   {
      case MENU_OPTION_SHIP_RETURN:
         Process_Place_Menu(player,
            BattleShip_UI_Place_Menu(player->grid.defense));
         break;
      case MENU_OPTION_SHIP_PLACE:
         {
            Coord_t location;
            Heading_t heading;
            BattleShip_UI_Read_Ship_Location_Heading(&location, &heading);
            Ship_t ship = (Ship_t)
            {
               .type = choice.type,
               .location = location,
               .heading = heading
            };
            Player_Place_Ship(player, &ship);
         }
         break;
   }
}

static void Process_Place_Menu(Player_t *player, Place_Menu_Option_t choice)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   if (player)
   {
      switch (choice)
      {
      case MENU_OPTION_PLACE_RETURN:
         return;
      case MENU_OPTION_PLACE_HELP:
         //BattleShip_UI_Print_Place_Help(); // TODO
         break;
      case MENU_OPTION_PLACE_AUTO:
         //BattleShip_UI_Ship_Auto(player);
         // TODO confirm yes/no
         {
            //Grid_Clear_Defense(player->grid);
            Grid_t *grid = &player->grid;
            if (grid)
            {
               memset(grid->defense, 0, grid->rows * grid->cols * sizeof(Grid_State_t));
            }
            for (uint i = 0; i < NUM_SHIPS; i++)
            {
               Grid_Status_t status = GRID_STATUS_UNKNOWN;
               do
               {
                  Coord_t location = Coord_Init_Random(
                      0, GRID_SIZE,
                      0, GRID_SIZE);
                  Heading_t heading = Heading_Init_Random();
                  Ship_t ship = (Ship_t)
                  {
                     .type = shipTable[i].type,
                     .location = location,
                     .heading = heading
                  };
                  status = Player_Place_Ship(player, &ship);
               }
               // TODO add status for null pointer exception
               while (GRID_STATUS_OK != status);
            }
            Process_Place_Menu(player,
                               BattleShip_UI_Place_Menu(player->grid.defense));
         }
         break;
      case MENU_OPTION_PLACE_MANUAL:
         Process_Ship_Menu(player,
                           BattleShip_UI_Ship_Menu_Manual(player->grid.defense));
      }
   }
}

static void Process_Main_Menu(Player_t *player, Main_Menu_Option_t choice)
{
#ifndef NDEBUG
   printf("\n%s\n", __FUNCTION__);
#endif
   switch(choice)
   {
      case MENU_OPTION_MAIN_RETURN:
         return;
      case MENU_OPTION_MAIN_PLACE:
         Process_Place_Menu(player,
               BattleShip_UI_Place_Menu(player->grid.defense));
         break;
      case MENU_OPTION_MAIN_BEGIN:
         break;
      case MENU_OPTION_MAIN_WATCH:
         break;
      case MENU_OPTION_MAIN_ABOUT:
         break;
   }
}

// Identify which ship was hit and determine if it has been sunk.
/*void BattleShip_Identify_Ship()
{
   // TODO
}*/

// state machine
/*void BattleShip_Ai()
{
   // TODO
}*/

// TODO define these
/*static void BattleShip_Logo_Print();
static void BattleShip_Grid_Print_2();
static void BattleShip_Grid_Print_1();
static void BattleShip_Set_Ship();
static void BattleShip_Clear_Ship();
static void BattleShip_Enter_Ship_Data();
static void BattleShip_Check_Ship_Grid();
static void BattleShip_Check_Ship_Collision();
static void BattleShip_Target_Print();
static void Strip_Spaces(); // for config read
 */
