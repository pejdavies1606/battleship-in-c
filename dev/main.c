/*
 * main.c
 *
 *  Created on: 25 Jun 2018
 *      Author: pejdavies1606
 *
 */

#include "BattleShipConfig.h"
#include "battleship/game.h"

int main(int argc, char *argv[])
{
   bool result = false;

   printf("%s Version %d.%d\n",
      argv[0],
      BattleShip_VERSION_MAJOR,
      BattleShip_VERSION_MINOR);
#ifdef DEBUG
   puts("DEBUG");
#endif
   setvbuf(stdout, NULL, _IONBF, 0); // No buffering

   Game_t game = {0};
   result = BattleShip_Game_Init(&game);
   if (result)
   {
      result = BattleShip_Game_Start(&game);
      BattleShip_Game_Destroy(&game);
   }

   return (result) ? 0 : 1;
}
