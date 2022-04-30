/*
 * util.c
 *
 *  Created on: 22 Oct 2018
 *      Author: pejdavies1606
 */

#include "battleship/util.h"

#include <string.h>

bool TrimStr(char * str, size_t str_size)
{
   bool result = false;
   if (str && str_size > 0)
   {
      size_t str_len = strnlen(str, str_size);
      if (str_len > 0 && str_len < str_size)
      {
         char *c;
         // truncate on newlines and carriage returns
         // "foo\n" -> "foo\0" -> "foo"
         while ((c = strpbrk(str, "\n")))
         {
            *c = 0;
         }
         // recalculate length
         // "foo \n " -> "foo \0 " -> "foo "
         str_len = strnlen(str, str_size);
         // truncate on trailing spaces and tabs
         // "foo " -> "foo\0" -> "foo"
         if (str_len > 0 && str_len < str_size)
         {
            c = &str[str_len - 1];
            while (*c == ' ' || *c == '\t')
            {
               *c = 0;
               c--;
            }
         }
         result = true;
      }
   }
   return result;
}

int CalcMax(const int *data, uint n)
{
   int max = data[0];
   for (uint i = 0; i < n; i++)
   {
      if (data[i] > max)
      {
         max = data[i];
      }
   }
   return max;
}

int CalcNumWidth(int n)
{
   int w = 0;
   while (n != 0)
   {
      n /= 10;
      ++w;
   }
   return w;
}

bool RepeatChar(char * line, size_t size_line, char elem)
{
   bool result = false;
   if (line && size_line > 0)
   {
      memset(line, elem, size_line);
      line[size_line - 1] = '\0';
      result = true;
   }
   return result;
}
