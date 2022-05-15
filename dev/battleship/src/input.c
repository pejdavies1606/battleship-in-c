#include "battleship/input.h"
#include "battleship/coord.h"

#include <errno.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

static bool _ParseCoord(const char *input, Coord_t *output);
static bool _ParseHeading(const char *input, Heading_e *output);

bool ParseInput(const char *str, InputData_t *data)
{
   bool result = false;
   if (str && data)
   {
      switch (data->type)
      {
      case INPUT_INT:
         result = ParseInt(str, &data->val.ival) &&
            ValidateRange(
               (int) data->val.ival,
               (int) data->min.ival,
               (int) data->max.ival);
         break;
      case INPUT_COORD:
         result = _ParseCoord(str, &data->val.loc) &&
            ValidateRange(
               data->val.loc.col,
               0,
               data->max.loc.col) &&
            ValidateRange(
               data->val.loc.row,
               0,
               data->max.loc.row);
         break;
      case INPUT_HEADING:
         result = _ParseHeading(str, &data->val.hdg);
         break;
      default:
         break;
      }
   }
   return result;
}

bool ParseInt(const char *str, int *val)
{
   bool result = false;
   char *endstr = NULL;
   long lval = 0;
   if (str && val)
   {
      errno = 0;
      lval = strtol(str, &endstr, 10); // radix 10 = decimal
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
      else if (errno != 0 && lval == 0)
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
      if (result)
      {
         *val = (int)lval;
      }
   }
   return result;
}

bool _ParseCoord(const char *str, Coord_t *loc)
{
   bool result = false;
   if (str && loc)
   {
      // [A-Z][0-9][0-9]
      result =
         Coord_ColFromChar(str[0], &loc->col) &&
         Coord_RowFromStr(&str[1], &loc->row);
   }
   return result;
}

bool _ParseHeading(const char * const str, Heading_e * const hdg)
{
   bool result = false;
   if (str && hdg)
   {
      char c = (char)toupper(str[0]);
      for (uint i = 0; i < NUM_HEADINGS; i++)
      {
         if (c == HEADING_TABLE[i].c)
         {
            *hdg = HEADING_TABLE[i].h;
            result = true;
            break;
         }
      }
   }
   return result;
}

bool ValidateRange(int val, int min, int max)
{
   bool result = false;
   result = (val >= min) && (val < max);
   return result;
}
