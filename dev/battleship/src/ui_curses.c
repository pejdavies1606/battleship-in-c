/*
 * ui_curses.c
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#include "battleship/form.h"
#include "battleship/form_curses.h"
#include "battleship/input.h"
#include "battleship/menu_curses.h"
#include "battleship/ui.h"
#include "battleship/util.h"

#include <limits.h>
#include <curses.h>
#include <stdio.h>

#define MENU_ROWS    10
#define PROMPT_ROW   11

void BattleShipUI_ClearScreen(void)
{
   clear();
   refresh();
}

void BattleShipUI_PrintLogo(void)
{
   for (int row = 0; row < NUM_LOGO_ROWS; row++)
   {
      char const *line = BattleShipUI_GetLogo(row);
      if (line)
      {
         mvaddstr(MENU_ROWS + row, 0, line);
      }
   }
   refresh();
}

bool BattleShipUI_PrintMessage(char const * const message)
{
   bool result = false;
   if (message)
   {
      addstr(message);
      result = true;
   }
   return result;
}

bool BattleShipUI_PrintGrid(
   Grid_t const * const def_grid,
   Grid_t const * const off_grid)
{
   bool result = false;
   static Line_t line = {0};
   if (def_grid)
   {
      result = true;
      for (int row = GRID_ROW_TITLE; row < (MAX_COORD_ROW + 1); row++)
      {
         if (Grid_GetRowStr(def_grid, &line, row, false))
         {
            if (off_grid)
            {
               line.buffer[line.position++] = ' ';
               if (!Grid_GetRowStr(off_grid, &line, row, true))
               {
                  result = false;
                  break;
               }
            }
            mvaddstr(MENU_ROWS + row, 0, line.buffer);
         }
         else
         {
            result = false;
            break;
         }
         memset(line.buffer, 0, MAX_BUFFER_SIZE);
         line.position = 0U;
      }
   }
   return result;
}

bool BattleShipUI_PrintMenu(Menu_t *menu)
{
   bool result = false;
   if (menu)
   {
      MenuData_t *data = (MenuData_t *)menu->data;
      if (data)
      {
         post_menu(data->menu);
         refresh();
         result = true;
      }
   }
   return result;
}

bool BattleShipUI_ReadMenu(Menu_t *menu, uint *choice)
{
   bool result = false;
   if (menu && choice)
   {
      *choice = menu->num_options;
      MenuData_t *data = (MenuData_t *)menu->data;
      if (data)
      {
         while (!result)
         {
            int c = getch();
            switch (c)
            {
            case KEY_DOWN:
               menu_driver(data->menu, REQ_DOWN_ITEM);
               break;
            case KEY_UP:
               menu_driver(data->menu, REQ_UP_ITEM);
               break;
            case '\n':
               result = true;
               break;
            default:
               break;
            }
         }

         unpost_menu(data->menu);
         refresh();

         ITEM *item = current_item(data->menu);
         for (uint i = 0U; i < menu->num_options; i++)
         {
            if (memcmp(item, data->items[i], sizeof(ITEM)) == 0)
            {
               *choice = i;
               break;
            }
         }
      }
   }
   return result;
}

bool BattleShipUI_ReadForm(Form_t *form, InputVal_t *val)
{
   bool result = false;
   if (form && val)
   {
      FormData_t *data = (FormData_t *)form->data;
      if (data)
      {
         mvaddstr(0, 0, form->title);
         post_form(data->form);
         refresh();

         form_driver(data->form, REQ_CLR_FIELD);
         while (!result)
         {
            int c = getch();
            switch (c)
            {
            case '\n':
               form_driver(data->form, REQ_VALIDATION);
               result = true;
               break;
            default:
               form_driver(data->form, c);
               break;
            }
         }

         unpost_form(data->form);
         refresh();

         result = ParseInput(
            field_buffer(data->fields[1], 0),
            form->fields[0].data);

         if (result)
         {
            *val = form->fields[0].data->val;
         }
      }
   }
   return result;
}