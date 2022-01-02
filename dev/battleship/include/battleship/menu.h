/*
 * menu.h
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef MENU_H_
#define MENU_H_

#include "battleship/commondefs.h"

typedef struct
{
   uint column_width_index; // depends on num_options
   uint *column_width_data; // num_headers, max width of header and options for that column
   uint *header_width_data; // num_headers, width of each header
   uint *option_width_data; // 2D array: row+col*num_rows = option+header*num_options
} Menu_Meta_t;

typedef struct
{
   char * title;
   uint num_headers;
   char * *headers;
   uint num_options;
   char * *options; // 2D array: row+col*num_rows = option+header*num_options
   Menu_Meta_t meta;
} Menu_t;

bool Menu_InitMeta(Menu_t *menu);

#endif /* MENU_H_ */
