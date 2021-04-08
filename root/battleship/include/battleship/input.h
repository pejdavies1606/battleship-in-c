/*
 * input.h
 *
 *  Created on: 1 Mar 2020
 *      Author: pejdavies1606
 */

#ifndef _INPUT_H_
#define _INPUT_H_

#include "battleship/commondefs.h"

typedef bool (*InputParser_t)(const String_t input, void *output);

bool ParseHeading(const String_t input, void *output);

#endif /* _INPUT_H_ */
