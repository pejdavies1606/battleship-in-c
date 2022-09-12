/*
 * menu_curses.c
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#include "battleship/menu.h"
#include "battleship/menu_curses.h"

bool Menu_InitData(Menu_t *const menu)
{
   bool result = false;
   if (menu)
   {
      if (menu->num_options > 0U)
      {
         ITEM **c_items = calloc(menu->num_options + 1U, sizeof(ITEM *));
         if (c_items)
         {
            result = true;
            for (uint i = 0U; i < menu->num_options; i++)
            {
               c_items[i] = new_item(menu->options[i], NULL);
               if (!c_items[i])
               {
                  result = false;
                  break;
               }
            }
         }
         if (result)
         {
            MENU *c_menu = new_menu(c_items);
            if (!c_menu)
            {
               result = false;
            }
            else
            {
               MenuData_t *data = malloc(sizeof(MenuData_t));
               if (!data)
               {
                  result = false;
               }
               else
               {
                  data->items = c_items;
                  data->menu = c_menu;
                  menu->data = (void *)data;
               }
            }
         }
         if (!result)
         {
            Menu_DestroyData(menu);
         }
      }
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
         for (uint i = 0U; i < menu->num_options; i++)
         {
            free_item(data->items[i]);
         }
         free_menu(data->menu);
         free(data);
      }
   }
}