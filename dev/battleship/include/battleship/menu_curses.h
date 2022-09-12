/*
 * menu_curses.h
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#ifndef MENU_CURSES_H_
#define MENU_CURSES_H_

#include <menu.h>

typedef struct MenuData
{
   ITEM **items;
   MENU *menu;
} MenuData_t;

#endif /* MENU_CURSES_H_ */
