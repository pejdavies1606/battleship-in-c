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

String_t rtrim(String_t str, size_t str_size)
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

void ReadString(String_t str, size_t str_size, FILE *stream)
{
   // Type-ahead cannot be avoided by attempting to 'flush' stdin.
   // http://c-faq.com/stdio/gets_flush2.html
   // http://c-faq.com/stdio/stdinflush2.html
   //int c; while ((c = getchar()) != '\n' && c != EOF);
   memset(str, 0, str_size);
   size_t line_size = str_size + 1; // to account for newline from fgets
   String_t line = malloc(line_size * sizeof(*line));
   if (fgets(line, (int) line_size, stream))
   {
      line = rtrim(line, line_size);
      strncpy(str, line, str_size);
#ifndef NDEBUG
      //printf("%s '%s'\n", __FUNCTION__, str);
#endif
      free(line);
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
