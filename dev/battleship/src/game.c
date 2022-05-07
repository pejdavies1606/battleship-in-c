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

static bool _ProcShipMenu(Player_t * const player, ShipMenuChoice_t const * const shipChoice);
static bool _ProcPlaceMenu(Player_t * const player, PlaceMenuOption_e const placeChoice);
static bool _ProcMainMenu(Game_t * const game, MainMenuOption_e const mainChoice);
static bool _ProcTurn(Coord_t const * const target, Player_t * const player);

bool BattleShip_Game_Init(Game_t * const game)
{
   bool result = false;
   if (game)
   {
      Window_Init(
          POS_WINDOW_X, POS_WINDOW_Y,
          SIZE_WINDOW_W, SIZE_WINDOW_H);

#ifdef DEBUG
      Rng_Init(1606);
#else
      Rng_Init(0);
#endif

      for (uint p = 0; p < NUM_PLAYERS; p++)
      {
         result = Player_Init(&game->players[p]);
         if (!result) break;
      }

      if (result)
      {
         game->ship_health.title = "Ship Health";
         Scoreboard_Init(&game->ship_health, NUM_SHIPS);
         for (uint i = 0; i < NUM_SHIPS; i++)
         {
            const Ship_Info_t ship_info = SHIP_TABLE[i];
            game->ship_health.entities[i].name = ship_info.name;
            game->ship_health.entities[i].total = ship_info.length;
         }

         game->hit_score.title = SCOREBOARD_HITS_TITLE;
         game->hit_score.num_entities = NUM_PLAYERS;
         game->hit_score.width_score = SCOREBOARD_HITS_SCORE_WIDTH;
         game->hit_score.width_total = SCOREBOARD_HITS_SCORE_WIDTH;

         result = BattleShipUI_Init();
      }
   }

   return result;
}

void BattleShip_Game_Destroy(Game_t * const game)
{
   if (game)
   {
      Scoreboard_Destroy(&game->hit_score);
      Scoreboard_Destroy(&game->ship_health);
      BattleShipUI_Destroy();
   }
}

bool BattleShip_Game_Start(Game_t * const game)
{
   bool result = false;
   MainMenuOption_e mainChoice = MENU_OPTION_MAIN_RETURN;
   if (game)
   {
      // initialise computer player
      result = Player_PlaceShipsAuto(&game->players[1]);
#ifdef DEBUG
      result = Player_PlaceShipsAuto(&game->players[0]);
#endif
      // enter UI for user player
      do
      {
#ifdef DEBUG
         mainChoice = MENU_OPTION_MAIN_BEGIN;
#else
         mainChoice = BattleShipUI_MainMenu("");
#endif
         result = _ProcMainMenu(game, mainChoice);
      }
      while (result && mainChoice != MENU_OPTION_MAIN_RETURN);
   }
   return result;
}

static bool _ProcShipMenu(Player_t * const player, ShipMenuChoice_t const * const shipChoice)
{
   bool result = false;
   if (player && shipChoice)
   {
      switch (shipChoice->option)
      {
      case MENU_OPTION_SHIP_RETURN:
         result = true;
         break;
      case MENU_OPTION_SHIP_PLACE:
      {
         GridStatus_e status = GRID_STATUS_UNKNOWN;
         Ship_t ship = {0};
         ship.type = shipChoice->type;
         bool loc = BattleShipUI_ReadCoord(&ship.location);
         bool hdg = BattleShipUI_ReadHeading(&ship.heading);
         if (loc && hdg)
         {
            result = Player_PlaceShip(player, &ship, &status);
            if (result)
            {
               result = (GRID_STATUS_OK == status);
            }
         }
      }
      break;
      }
   }
   return result;
}

static bool _ProcPlaceMenu(Player_t *player, PlaceMenuOption_e placeChoice)
{
   ShipMenuChoice_t shipChoice = {0};
   bool result = false;
   if (player)
   {
      switch (placeChoice)
      {
      case MENU_OPTION_PLACE_RETURN:
         result = true;
         break;
      case MENU_OPTION_PLACE_HELP:
         //BattleShip_UI_Print_Place_Help(); // TODO
         break;
      case MENU_OPTION_PLACE_AUTO:
         // TODO confirm yes/no
         result = Player_PlaceShipsAuto(player);
         break;
      case MENU_OPTION_PLACE_MANUAL:
         do
         {
            shipChoice = BattleShipUI_ShipMenuManual(&player->grid);
            result = _ProcShipMenu(player, &shipChoice);
         }
         while (shipChoice.option != MENU_OPTION_SHIP_RETURN);
         break;
      }
   }
   return result;
}

static bool _ProcBeginGame(Game_t * const game)
{
   bool result = false;
   int round = 0;
   Coord_t target = {0};
   if (game)
   {
      bool stop = false;
      do
      {
         result = true;
         BattleShipUI_GameScreen(game);
         result = BattleShipUI_ReadCoord(&target);
         if (!result) break;
         result = _ProcTurn(&target, &game->players[1]);
#if 0
         result = BattleShipAI_GetCoord(&target);
         if (!result) break;
         result = _ProcTurn(&target, &game->players[0]);
#endif
         round++;
         // TODO check all ships sunk
      }
      while (result && !stop);
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

static bool _ProcMainMenu(Game_t * const game, MainMenuOption_e const mainChoice)
{
   PlaceMenuOption_e placeChoice = MENU_OPTION_PLACE_RETURN;
   Player_t * player = NULL;
   bool result = false;
   if (game && game->players)
   {
      player = &game->players[0];
      switch (mainChoice)
      {
      case MENU_OPTION_MAIN_RETURN:
         result = true;
         break;
      case MENU_OPTION_MAIN_PLACE:
         do
         {
            placeChoice = BattleShipUI_PlaceMenu(&player->grid);
            result = _ProcPlaceMenu(player, placeChoice);
         }
         while (result && placeChoice != MENU_OPTION_PLACE_RETURN);
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

bool _ProcTurn(
   Coord_t const * const target,
   Player_t * const player)
{
   bool result = false;
   if (target && player)
   {
      ShipType_e hit_ship = SHIP_NONE;
      result = Grid_PlaceHit(
          &player->grid,
          player->grid.states,
          target, &hit_ship);
      if (result && hit_ship != SHIP_NONE)
      {
         bool sunk = false;
         result = Player_HitShip(player, hit_ship, &sunk);
         if (result && sunk)
         {
            // TODO print message
         }
      }
   }
   return result;
}

// state machine
/*void BattleShip_Ai() // TODO
{
}*/
