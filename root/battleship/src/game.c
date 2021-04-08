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

#include "battleship/game.h"
#include "battleship/window.h"
#include "battleship/ui.h"
#include "battleship/rng.h"

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

Game_t * BattleShip_Game_Init(void)
{
#ifndef NDEBUG
   printf("%s\n", __FUNCTION__);
#endif
   Window_Init(
      POS_WINDOW_X, POS_WINDOW_Y,
      SIZE_WINDOW_W, SIZE_WINDOW_H);
#ifndef NDEBUG
   Rng_Init(1606);
#else
   Rng_Init(0);
#endif

   Game_t *game = malloc(sizeof(Game_t));
   if (!game)
   {
      return NULL;
   }
   memset(game, 0, sizeof(Game_t));

   game->ship_health.title = "Ship Health";
   Scoreboard_Init(&game->ship_health, NUM_SHIPS);
   for (uint i = 0; i < NUM_SHIPS; i++)
   {
      const Ship_Info_t ship_info = shipTable[i];
      game->ship_health.entities[i].name = ship_info.name;
      game->ship_health.entities[i].total = ship_info.length;
   }

   game->hit_score.title = SCOREBOARD_HITS_TITLE;
   game->hit_score.num_entities = NUM_PLAYERS;
   game->hit_score.width_score = SCOREBOARD_HITS_SCORE_WIDTH;
   game->hit_score.width_total = SCOREBOARD_HITS_SCORE_WIDTH;

   game->players = Player_Init(NUM_PLAYERS);
   if (!game->players)
   {
      return NULL;
   }

   BattleShip_UI_Init();
   return game;
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
   printf("%s\n", __FUNCTION__);
#endif
   switch(choice.option)
   {
      case MENU_OPTION_SHIP_RETURN:
         Process_Place_Menu(player,
            BattleShip_UI_Place_Menu(&player->grid));
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
   printf("%s\n", __FUNCTION__);
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
         // TODO confirm yes/no
         Player_Place_Ships_Auto(player);
         Process_Place_Menu(
            player,
            BattleShip_UI_Place_Menu(&player->grid));
         break;
      case MENU_OPTION_PLACE_MANUAL:
         Process_Ship_Menu(
            player,
            BattleShip_UI_Ship_Menu_Manual(&player->grid));
      }
   }
}

static void Process_Main_Menu(Player_t *player, Main_Menu_Option_t choice)
{
#ifndef NDEBUG
   printf("%s\n", __FUNCTION__);
#endif
   switch(choice)
   {
      case MENU_OPTION_MAIN_RETURN:
         return;
      case MENU_OPTION_MAIN_PLACE:
         Process_Place_Menu(player,
               BattleShip_UI_Place_Menu(&player->grid));
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
