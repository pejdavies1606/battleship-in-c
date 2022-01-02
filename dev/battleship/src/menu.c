/*
 * menu.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/menu.h"
#include "battleship/util.h"

#include <string.h>

bool Menu_InitMeta(Menu_t * const menu)
{
   bool result = false;
   if (menu &&
       menu->num_headers > 0 &&
       menu->num_options > 0)
   {
      menu->meta.column_width_index = (uint)CalcNumWidth((int)menu->num_options);
      uint *column_widths = malloc(menu->num_headers * sizeof(uint));
      uint *header_widths = malloc(menu->num_headers * sizeof(uint));
      uint *option_widths = malloc(menu->num_options * menu->num_headers * sizeof(uint));
      if (column_widths && header_widths && option_widths)
      {
         result = true;
         // loop headers
         for (uint header_index = 0; header_index < menu->num_headers; header_index++)
         {
            // get size of header string
            size_t header_len = strnlen(menu->headers[header_index], SIZE_MENU_COL_MAX);
            if (header_len > 0 && header_len < SIZE_MENU_COL_MAX)
            {
               header_widths[header_index] = (uint)header_len;
               // loop options
               for (uint option_index = 0; option_index < menu->num_options; option_index++)
               {
                  uint i = header_index * menu->num_options + option_index;
                  // get size of option string
                  size_t option_len = strnlen(menu->options[i], SIZE_MENU_COL_MAX);
                  if (option_len > 0 && option_len < SIZE_MENU_COL_MAX)
                  {
                     option_widths[i] = (uint)option_len;
                  }
                  else
                  {
                     result = false;
                     break;
                  }
               }
               // get maximum size of option strings
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
            else
            {
               result= false;
               break;
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
