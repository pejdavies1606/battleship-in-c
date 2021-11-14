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

bool Menu_Meta_Init(Menu_t *menu)
{
   bool result = false;
   if (menu)
   {
      menu->meta.column_width_index = (uint)CalcNumWidth((int)menu->num_options);
      uint *column_widths = malloc(
          menu->num_headers * sizeof(uint));
      uint *header_widths = malloc(
          menu->num_headers * sizeof(uint));
      uint *option_widths = malloc(
          menu->num_options * menu->num_headers * sizeof(uint));
      if (column_widths && header_widths && option_widths)
      {
         for (uint header_index = 0; header_index < menu->num_headers; header_index++)
         {
            header_widths[header_index] = (uint)strlens(menu->headers[header_index]);
            for (uint option_index = 0; option_index < menu->num_options; option_index++)
            {
               uint i = header_index * menu->num_options + option_index;
               option_widths[i] = (uint)strlens(menu->options[i]);
            }
            uint option_width_max = (uint)CalcMax(
                (int *)&option_widths[header_index * menu->num_options],
                menu->num_options);
            if (option_width_max > header_widths[header_index])
            {
               column_widths[header_index] = option_width_max;
            }
            else
            {
               column_widths[header_index] = header_widths[header_index];
            }
         }
         menu->meta.column_width_data = column_widths;
         menu->meta.header_width_data = header_widths;
         menu->meta.option_width_data = option_widths;
         result = true;
      }
   }
   return result;
}

