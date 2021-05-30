/*
 * util.c
 *
 *  Created on: 22 Oct 2018
 *      Author: pejdavies1606
 */

#include "battleship/util.h"

#include <errno.h>
#include <limits.h>
#include <string.h>

String_t TrimStr(String_t str, size_t str_size)
{
   size_t str_len = strnlen(str, str_size);
   if (str && str_size > 0 && str_len > 0 && str_len < str_size)
   {
      String_t c;
      // truncate on newlines and carriage returns
      // "foo\n" -> "foo\0" -> "foo"
      while ((c = strpbrk(str, "\n\r")))
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
   }
   return str;
}

bool ParseUnsignedLong(const char *str, unsigned long *val)
{
    bool result = false;
    char *endstr = NULL;
    errno = 0;
    if (str && val)
    {
       *val = strtoul(str, &endstr, 10); // radix 10 = decimal

       if (endstr == str)
       {
          result = false; // invalid  (no digits found, 0 returned)
       }
       else if (errno == ERANGE)
       {
          result = false; // invalid  (underflow or overflow occurred)
       }
       else if (errno == EINVAL)  /* not in all c99 implementations - gcc OK */
       {
          result = false; // invalid  (base contains unsupported value)
       }
       else if (errno != 0 && *val == 0)
       {
          result = false; // invalid  (unspecified error occurred)
       }
       else if (errno == 0 && str && !*endstr)
       {
          result = true; // valid  (and represents all characters read)
       }
       else if (errno == 0 && str && *endstr != 0)
       {
          result = true; // valid  (but additional characters remain)
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

int RepeatChar(String_t line, size_t size_line, char elem)
{
   if ( ! memset(line, elem, size_line) )
   {
      return -1;
   }
   line[size_line - 1] = '\0';
   return 0;
}
