#include "battleship/input.h"
#include "battleship/coord.h"

#include <errno.h>
#include <limits.h>
#include <string.h>

bool ParseInput(const char *str, InputData_t *data)
{
   bool result = false;
   if (str && data)
   {
      switch (data->type)
      {
      case INPUT_ULONG:
         result = ParseUnsignedLong(str, &data->val.ul);
         break;
      case INPUT_COORD_ROW:
         break;
      case INPUT_COORD_COL:
         break;
      case INPUT_HEADING:
         result = ParseHeading(str, &data->val.hdg);
         break;
      default:
         break;
      }
   }
   return result;
}

bool ParseUnsignedLong(const char *str, unsigned long *val)
{
   bool result = false;
   char *endstr = NULL;
   if (str && val)
   {
      errno = 0;
      *val = strtoul(str, &endstr, 10); // radix 10 = decimal
      if (endstr == str)
      {
         result = false; // invalid  (no digits found, 0 returned)
      }
      else if (errno == ERANGE)
      {
         result = false; // invalid  (underflow or overflow occurred)
      }
      else if (errno == EINVAL) /* not in all c99 implementations - gcc OK */
      {
         result = false; // invalid  (base contains unsupported value)
      }
      else if (errno != 0 && *val == 0)
      {
         result = false; // invalid  (unspecified error occurred)
      }
      else if (errno == 0 && endstr && *endstr == '\0')
      {
         result = true; // valid  (and represents all characters read)
      }
      else if (errno == 0 && endstr && *endstr != '\0')
      {
         result = true; // valid  (but additional characters remain)
      }
   }
   return result;
}

bool ParseHeading(const char *str, Heading_e *hdg)
{
   bool result = false;
   if (str && hdg && strlens(str) <= LEN_HEADING)
   {
      for (uint i = 0; i < NUM_HEADINGS; i++)
      {
         if (0 == strncmp(
            str,
            headingTable[i].str,
            LEN_HEADING))
         {
            *hdg = headingTable[i].hdg;
            result = true;
            break;
         }
      }
   }
   return result;
}
