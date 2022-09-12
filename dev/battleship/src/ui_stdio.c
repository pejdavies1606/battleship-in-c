/*
 * ui_stdio.c
 *
 *  Created on: 9 April 2021
 *      Author: pejdavies1606
 */

#include <stdio.h>
#include <limits.h>
#include "battleship/menu_stdio.h"
#include "battleship/util.h"
#include "battleship/ui.h"
#include "battleship/input.h"

// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
// Borland-style CONIO implementation for MinGW/Dev-C++ (http://conio.sourceforge.net/)
#if defined(CONIO)
#include "conio21/conio2.h"
#endif

static bool _ReadInput(
   char * prompt,
   size_t option_len,
   InputData_t *data);

static bool _ReadString(char * str, size_t str_size, FILE *stream);

void BattleShipUI_ClearScreen(void)
{
#ifdef DEBUG
   puts("clrscr");
#else
#if defined(CONIO)
   clrscr();
#else
   printf("\e[1;1H\e[2J")
#endif
#endif
}

void BattleShipUI_PrintLogo(void)
{
   char const *line = NULL;
   int row = 0;
#ifdef DEBUG
   UNUSED(line);
   UNUSED(row);
   puts("logo");
#else
   for (row = 0; row < NUM_LOGO_ROWS; row++)
   {
      line = BattleShipUI_GetLogo(row);
      if (line)
      {
         puts(line);
      }
   }
#endif
}

bool BattleShipUI_PrintMessage(char const * const message)
{
   bool result = false;
   if (message)
   {
      puts(message);
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
            printf("%s\n", line.buffer);
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
      MenuData_t *data = menu->data;
      if (data)
      {
         puts("");
         printf("%*s%s\n", data->column_width_index, "", menu->title);
         printf("%*s%s", data->column_width_index, "", "#");
         for (uint col = 0; col < menu->num_headers; col++)
         {
            uint column_width =
                (col == 0)
                    ? data->column_width_index
                    : (data->column_widths[col - 1] -
                       data->header_widths[col - 1] + 1);
            printf("%*s%s", column_width, "",
                   IF_NULL_VAL(menu->headers[col], ""));
         }
         printf("\n");
         for (uint row = 0; row < menu->num_options; row++)
         {
            printf("%*s%d", data->column_width_index, "", row);
            for (uint col = 0; col < menu->num_headers; col++)
            {
               uint column_width =
                   (col == 0)
                       ? data->column_width_index
                       : (data->column_widths[col - 1] -
                          data->option_widths[row + (col - 1) * menu->num_options] + 1);
               printf("%*s%s", column_width, "",
                      IF_NULL_VAL(menu->options[row + col * menu->num_options], ""));
            }
            printf("\n");
         }
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
      MenuData_t *data = menu->data;
      if (data)
      {
         InputData_t option = {0};
         option.type = INPUT_INT;
         option.max.ival = (int)menu->num_options;
         result = _ReadInput(
             "option",
             data->column_width_index,
             &option);
         if (result)
         {
            *choice = (uint)option.val.ival;
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
      if (form->fields)
      {
         Field_t *field = &form->fields[0];
         result = _ReadInput(field->prompt, field->len, field->data);
         if (result)
         {
            *val = form->fields[0].data->val;
         }
      }
   }
   return result;
}

bool _ReadInput(
   char * prompt,
   size_t option_len,
   InputData_t *data)
{
   bool result = false;
   if (prompt && option_len > 0 && data)
   {
      uint retries = 0;
      while (!result && retries < MAX_READ_RETRIES)
      {
#ifdef DEBUG
         printf("%d Enter %s: ", retries, prompt);
#else
         printf("Enter %s: ", prompt);
#endif
         static char line[MAX_BUFFER_SIZE];
         result = _ReadString(
                      line,
                      MAX_BUFFER_SIZE,
                      stdin) &&
                  ParseInput(
                      line,
                      data);
         if (!result)
         {
            retries++;
         }
      }
   }
   return result;
}

bool _ReadString(char * str, size_t str_size, FILE *stream)
{
   bool result = false;
   // Type-ahead cannot be avoided by attempting to 'flush' stdin.
   // http://c-faq.com/stdio/gets_flush2.html
   // http://c-faq.com/stdio/stdinflush2.html
   //int c; while ((c = getchar()) != '\n' && c != EOF);
   if (str && str_size > 0 && stream)
   {
      memset(str, 0, str_size);
      if (fgets(str, (int)str_size, stream))
      {
         result = TrimStr(str, str_size);
      }
   }
   return result;
}