/*
 * util.c
 *
 *  Created on: 22 Oct 2018
 *      Author: pejdavies1606
 */

#include "util.h"

#include <errno.h>
#include <limits.h>
#include <string.h>

void ReadString(char *str, int strlen, FILE *stream)
{
   fgets(str, strlen, stream); // includes newline
   // Clear input buffer to prevent type-ahead
   // https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c/9750394
   if (str[0] != '\n')
   {
      int c;
      while ((c = getchar()) != '\n' && c != EOF);
   }
}

bool ParseUnsignedLong(const char *str, unsigned long *val)
{
    bool ret = true;
    char *endstr = NULL;
    errno = 0;
    *val = strtoul(str, &endstr, 10); // radix 10 = decimal

    if (str == endstr)
       ret = false; // invalid  (no digits found, 0 returned)
    else if (errno == ERANGE)
       ret = false; // invalid  (underflow or overflow occurred)
    else if (errno == EINVAL)  /* not in all c99 implementations - gcc OK */
       ret = false; // invalid  (base contains unsupported value)
    else if (errno != 0 && *val == 0)
       ret = false; // invalid  (unspecified error occurred)
    else if (errno == 0 && str && !*endstr)
       ret = true; // valid  (and represents all characters read)
    else if (errno == 0 && str && *endstr != 0)
       ret = true; // valid  (but additional characters remain)

    return ret;
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
