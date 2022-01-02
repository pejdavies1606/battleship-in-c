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

static bool _ProcShipMenu(Player_t *player, Ship_Menu_Choice_t choice);
static bool _ProcPlaceMenu(Player_t *player, PlaceMenuOption_e choice);
static bool _ProcMainMenu(Game_t * const game, MainMenuOption_e const choice);

Game_t * BattleShip_Game_Init(void)
{
   Window_Init(
      POS_WINDOW_X, POS_WINDOW_Y,
      SIZE_WINDOW_W, SIZE_WINDOW_H);

#ifdef DEBUG
   Rng_Init(1606);
#else
   Rng_Init(0);
#endif

   Game_t *game = malloc(sizeof(Game_t));
   if (game)
   {
      memset(game, 0, sizeof(Game_t));

      game->players = Player_Init(NUM_PLAYERS);
      if (!game->players)
      {
         free(game);
         game = NULL;
      }
      else
      {
         game->ship_health.title = "Ship Health";
         Scoreboard_Init(&game->ship_health, NUM_SHIPS);
         for (uint i = 0; i < NUM_SHIPS; i++)
         {
            Ship_Info_t const ship_info = SHIP_TABLE[i];
            game->ship_health.entities[i].name = ship_info.name;
            game->ship_health.entities[i].total = ship_info.length;
         }

         game->hit_score.title = SCOREBOARD_HITS_TITLE;
         game->hit_score.num_entities = NUM_PLAYERS;
         game->hit_score.width_score = SCOREBOARD_HITS_SCORE_WIDTH;
         game->hit_score.width_total = SCOREBOARD_HITS_SCORE_WIDTH;

         BattleShipUI_Init();
      }
   }

   return game;
}

bool BattleShip_Game_Start(Game_t *game)
{
   bool result = false;
   if (game)
   {
      result = Player_PlaceShipsAuto(&game->players[1]);
      while (result)
      {
         result = _ProcMainMenu(game, BattleShipUI_MainMenu(""));
      }
   }
   return result;
}

static bool _ProcShipMenu(Player_t *player, Ship_Menu_Choice_t choice)
{
   bool result = false;
   if (player)
   {
      switch (choice.option)
      {
      case MENU_OPTION_SHIP_RETURN:
         result = _ProcPlaceMenu(
             player,
             BattleShipUI_PlaceMenu(&player->grid));
         break;
      case MENU_OPTION_SHIP_PLACE:
      {
         Coord_t location;
         Heading_e heading;
         if (BattleShipUI_ReadShipCoord(&location, &heading))
         {
            Ship_t ship = (Ship_t){
                .type = choice.type,
                .location = location,
                .heading = heading};
            result = (GRID_STATUS_OK == Player_PlaceShip(player, &ship));
         }
      }
      break;
      }
   }
   return result;
}

static bool _ProcPlaceMenu(Player_t *player, PlaceMenuOption_e choice)
{
   bool result = false;
   if (player)
   {
      switch (choice)
      {
      case MENU_OPTION_PLACE_RETURN:
         result = true;
         break;
      case MENU_OPTION_PLACE_HELP:
         //BattleShip_UI_Print_Place_Help(); // TODO
         break;
      case MENU_OPTION_PLACE_AUTO:
         // TODO confirm yes/no
         result = Player_PlaceShipsAuto(player) &&
                  _ProcPlaceMenu(
                      player,
                      BattleShipUI_PlaceMenu(&player->grid));
         break;
      case MENU_OPTION_PLACE_MANUAL:
         result = _ProcShipMenu(
            player,
            BattleShipUI_ShipMenuManual(&player->grid));
         break;
      }
   }
   return result;
}

static bool _ProcBeginGame(Game_t * const game)
{
   bool doLoop = false;
   bool result = false;
   if (game)
   {
      doLoop = true;
      while (doLoop)
      {
         BattleShipUI_GameScreen(
            &game->players[0].grid,
            game->players[1].grid.hits);
         BattleShipUI_GameScreen(
            &game->players[1].grid,
            game->players[0].grid.hits);
         // TODO check all ships sunk
         doLoop = false;
      }
   }
   /* loop
    *    increment round counter
    *    update screen
    *    player turns
    *       manual
    *          read location
    *       auto
    *          random stage
    *          circle stage
    *            line stage
    */
   return result;
}

static bool _ProcMainMenu(Game_t * const game, MainMenuOption_e const choice)
{
   Player_t * player = NULL;
   bool result = false;
   if (game)
   {
      player = &game->players[0];
      switch (choice)
      {
      case MENU_OPTION_MAIN_RETURN:
         break;
      case MENU_OPTION_MAIN_PLACE:
         result = _ProcPlaceMenu(
             player,
             BattleShipUI_PlaceMenu(&player->grid));
         break;
      case MENU_OPTION_MAIN_BEGIN:
         result = _ProcBeginGame(game);
         break;
      case MENU_OPTION_MAIN_WATCH:
         break;
      case MENU_OPTION_MAIN_ABOUT:
         break;
      }
   }
   return result;
}

// Identify which ship was hit and determine if it has been sunk.
/*void BattleShip_Identify_Ship() // TODO
{
}*/

// state machine
/*void BattleShip_Ai() // TODO
{
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
