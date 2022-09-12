/*
 * menu.h
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef MENU_H_
#define MENU_H_

#include "battleship/commondefs.h"

#define SIZE_MENU_COL_MAX 32

typedef struct Menu
{
   char *title;
   uint num_headers;
   char **headers;
   uint num_options;
   char **options; // 2D array: row+col*num_rows = option+header*num_options
   void *data; // implementation specific
} Menu_t;

bool Menu_InitData(Menu_t *const menu);
void Menu_DestroyData(Menu_t *const menu);

#endif /* MENU_H_ */
