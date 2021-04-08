/*
 * ship.c
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/ship.h"

Ship_t * Ship_Init(uint num_ships)
{
   Ship_t *ships = malloc(num_ships * sizeof(Ship_t));
   if (!ships)
   {
      return NULL;
   }
   memset(ships, 0, num_ships * sizeof(Ship_t));
   return ships;
}
