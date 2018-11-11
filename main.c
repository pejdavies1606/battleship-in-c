/*
 * main.c
 *
 *  Created on: 25 Jun 2018
 *      Author: pejdavies1606
 *
 */

#include "battleships_game.h"

int main(int argc, char *argv[])
{
   puts("hello world");
   setvbuf(stdout, NULL, _IONBF, 0); // No buffering
   Battleships_Game_Init();
   Battleships_Game_Start();
   puts("done");
   getchar();
   return 0;
}
