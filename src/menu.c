/*
 * menu.c
 *
 *  Created on: 8 Jul 2018
 *      Author: pejdavies1606
 */

#include "menu.h"
#include "util.h"

#include <string.h>
#include <stdarg.h>

#define STR_LEN_MAX 32

static int Max(uint *max, uint size, uint *data);

void Menu_Init(
      Menu_t *menu,
      String_t title,
      uint num_headers,
      String_t *headers,
      uint num_options,
      String_t *options
      )
{
   menu->title = title;
   menu->num_headers = num_headers;
   menu->headers = headers;
   menu->num_options = num_options;
   menu->options = options;
}

void Menu_Meta_Init(Menu_Meta_t *meta, Menu_t *menu)
{
#ifdef DEBUG
   puts("Menu_Meta_Init");
   printf("title=%s\n", menu->title );
#endif
   // calculate number width from number of options
   uint n = menu->num_options;
   uint index_width = 0;
   while (n != 0)
   {
      n /= 10;
      ++index_width;
   }
   meta->column_width_index = index_width;

#ifdef DEBUG
   printf("index_width=%u\n", index_width );
#endif

   uint *header_widths = malloc( menu->num_headers * sizeof(*header_widths) );
   if (NULL == header_widths )
   {
      meta = NULL;
      return;
   }
   uint *option_widths = malloc( menu->num_options * menu->num_headers * sizeof(*option_widths) );
   if (NULL == option_widths )
   {
      meta = NULL;
      return;
   }
   for (uint header_index=0; header_index<menu->num_headers; header_index++)
   {
      header_widths[header_index] = strnlen(menu->headers[header_index], STR_LEN_MAX);

      for (uint option_index=0; option_index<menu->num_options; option_index++)
      {
         uint i = header_index*menu->num_options+option_index;
         option_widths[i] = strnlen(menu->options[i], STR_LEN_MAX);
#ifdef DEBUG
         //printf("option_widths[%u]=%u\n", i, option_widths[i] );
#endif
      }
      uint option_width_max = 0;
      if (Max(&option_width_max, menu->num_options, &option_widths[header_index*menu->num_options]) != 0)
      {
         meta = NULL;
         return;
      }
      if (option_width_max > header_widths[header_index])
      {
         header_widths[header_index] = option_width_max;
      }
#ifdef DEBUG
      printf("header_widths[%u]=%u\n", header_index, header_widths[header_index] );
#endif
   }
   meta->column_width_data = header_widths;
   meta->option_width_data = option_widths;
}

static int Max(uint *max, uint size, uint *data)
{
   if (NULL == data)
   {
      return -1;
   }

   *max = data[0];
   for (uint i=0; i<size; i++)
   {
      if (data[i] > *max)
      {
         *max = data[i];
      }
   }

   return 0;
}
