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
   setvbuf(stdout, NULL, _IONBF, 0); // No buffering
   BattleShip_Game_Init();
   BattleShip_Game_Start();
   puts("done");
   getchar();
   return 0;
}
