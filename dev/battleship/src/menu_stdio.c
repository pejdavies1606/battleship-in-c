/*
 * menu_stdio.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include "battleship/menu.h"
#include "battleship/menu_stdio.h"
#include "battleship/util.h"

#include <string.h>

bool Menu_InitData(Menu_t *const menu)
{
   MenuData_t *data = NULL;
   bool result = false;
   if (menu)
   {
      result = (menu->num_headers > 0U && menu->num_options > 0U &&
                menu->headers && menu->options);
      if (result)
      {
         data = malloc(sizeof(MenuData_t));
         if (data)
         {
            data->column_width_index =
                (uint)CalcNumWidth((int)menu->num_options);
            data->column_widths = calloc(
                menu->num_headers, sizeof(uint));
            data->header_widths = calloc(
                menu->num_headers, sizeof(uint));
            data->option_widths = calloc(
                menu->num_headers * menu->num_options, sizeof(uint));
            result = (data->column_widths &&
                      data->header_widths &&
                      data->option_widths);
         }
      }
      if (result)
      {
         // loop headers
         for (uint header_index = 0;
              header_index < menu->num_headers;
              header_index++)
         {
            if (menu->headers[header_index])
            {
               // get size of header string
               size_t header_len = strnlen(
                   menu->headers[header_index], SIZE_MENU_COL_MAX);
               if (header_len > 0 && header_len < SIZE_MENU_COL_MAX)
               {
                  data->header_widths[header_index] = (uint)header_len;
                  // loop options
                  for (uint option_index = 0;
                       option_index < menu->num_options;
                       option_index++)
                  {
                     uint i = header_index * menu->num_options + option_index;
                     if (menu->options[i])
                     {
                        // get size of option string
                        size_t option_len = strnlen(
                            menu->options[i], SIZE_MENU_COL_MAX);
                        if (option_len > 0 && option_len < SIZE_MENU_COL_MAX)
                        {
                           data->option_widths[i] = (uint)option_len;
                        }
                        else
                        {
                           result = false;
                           break;
                        }
                     }
                  }
                  // get maximum size of option strings
                  uint option_width_max = (uint)CalcMax(
                      (int *)&data->option_widths[header_index * menu->num_options],
                      menu->num_options);
                  data->column_widths[header_index] =
                      (option_width_max > data->header_widths[header_index])
                          ? option_width_max
                          : data->header_widths[header_index];
               }
               else
               {
                  result = false;
                  break;
               }
            }
         }
      }
      menu->data = (result) ? data : NULL;
   }
   return result;
}

void Menu_DestroyData(Menu_t *const menu)
{
   if (menu)
   {
      MenuData_t *data = menu->data;
      if (data)
      {
         free(data->column_widths);
         free(data->header_widths);
         free(data->option_widths);
      }
      free(data);
      data = NULL;
   }
}