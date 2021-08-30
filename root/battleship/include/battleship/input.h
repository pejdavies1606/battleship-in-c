/*
 * input.h
 *
 *  Created on: 1 Mar 2020
 *      Author: pejdavies1606
 */

#ifndef _INPUT_H_
#define _INPUT_H_

#include "battleship/commondefs.h"
#include "battleship/coord.h"

typedef enum InputType
{
  INPUT_ULONG,
  INPUT_COORD_COL,
  INPUT_COORD_ROW,
  INPUT_HEADING
} InputType_e;

typedef union InputVal
{
  unsigned long ul;
  Coord_t loc;
  Heading_e hdg;
} InputVal_t;

typedef struct InputData
{
  InputType_e type;
  InputVal_t val;
  InputVal_t min;
  InputVal_t max;
} InputData_t;

bool ParseInput(const char *input, InputData_t *output);

bool ParseUnsignedLong(const char *input, unsigned long *output);
bool ParseHeading(const char *input, Heading_e *output);

#endif /* _INPUT_H_ */
