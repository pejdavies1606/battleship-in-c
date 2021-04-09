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
   printf("%s Version %d.%d\n",
      argv[0],
      BattleShip_VERSION_MAJOR,
      BattleShip_VERSION_MINOR);
#ifdef DEBUG
   puts("DEBUG");
#endif
   setvbuf(stdout, NULL, _IONBF, 0); // No buffering

   Game_t *game = BattleShip_Game_Init();
   if (!game)
   {
      return STATUS_ERROR;
   }
   BattleShip_Game_Start(game);

   puts("done");
   getchar();
   return 0;
}
