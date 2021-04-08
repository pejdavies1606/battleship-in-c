/*
 * menu.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/menu.h"
#include "battleship/util.h"

#include <string.h>
#include <stdarg.h>

void Menu_Meta_Init(Menu_t *menu)
{
#ifndef NDEBUG
   //printf("\n%s\n", __FUNCTION__);
   //printf("title=%s\n", menu->title );
#endif
   menu->meta.column_width_index = (uint)CalcNumWidth((int)menu->num_options);
#ifndef NDEBUG
   //printf("index_width=%u\n", index_width );
#endif
   uint *column_widths = malloc( menu->num_headers * sizeof(*column_widths) );
   uint *header_widths = malloc( menu->num_headers * sizeof(*header_widths) );
   uint *option_widths = malloc( menu->num_options * menu->num_headers * sizeof(*option_widths) );
   for (uint header_index=0; header_index<menu->num_headers; header_index++)
   {
      header_widths[header_index] = (uint) strlens(menu->headers[header_index]);
#ifndef NDEBUG
      //printf("header_widths[%u](%s)=%u\n", header_index, IF_NULL_VAL(menu->headers[header_index],""), header_widths[header_index] );
#endif
      for (uint option_index=0; option_index<menu->num_options; option_index++)
      {
         uint i = header_index*menu->num_options+option_index;
         option_widths[i] = (uint) strlens(menu->options[i]);
#ifndef NDEBUG
         //printf("option_widths[%u](%s)=%u\n", i, IF_NULL_VAL(menu->options[i],""), option_widths[i] );
#endif
      }
      uint option_width_max = (uint)CalcMax((int*)&option_widths[header_index*menu->num_options], menu->num_options);
      if (option_width_max > header_widths[header_index])
      {
         column_widths[header_index] = option_width_max;
      }
      else
      {
         column_widths[header_index] = header_widths[header_index];
      }
#ifndef NDEBUG
      //printf("column_widths[%u]=%u\n", header_index, column_widths[header_index] );
#endif
   }
   menu->meta.column_width_data = column_widths;
   menu->meta.header_width_data = header_widths;
   menu->meta.option_width_data = option_widths;
}

