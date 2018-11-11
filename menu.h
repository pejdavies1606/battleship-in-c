/*
 * menu.h
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef MENU_H_
#define MENU_H_

#include "commondefs.h"

typedef struct
{
   String_t title;
   uint num_headers;
   String_t *headers;
   uint num_options;
   String_t *options; // 2D array: row*num_cols+col = header*num_options+option
} Menu_t;

typedef struct
{
   uint column_width_index;
   uint *column_width_data;
   uint *option_width_data; // 2D array: row*num_cols+col = header*num_options+option
} Menu_Meta_t;

void Menu_Init(Menu_t *menu, String_t title, uint num_headers, String_t *headers, uint num_options, String_t *options);

void Menu_Meta_Init(Menu_Meta_t *meta, Menu_t *menu);

#endif /* MENU_H_ */
