/*
 * menu_stdio.h
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#ifndef MENU_STDIO_H_
#define MENU_STDIO_H_

#include "battleship/commondefs.h"

typedef struct MenuData
{
   uint column_width_index; // depends on num_options
   uint *column_widths; // num_headers, max width of header and options for that column
   uint *header_widths; // num_headers, width of each header
   uint *option_widths; // 2D array: row+col*num_rows = option+header*num_options
} MenuData_t;

#endif /* MENU_STDIO_H_ */