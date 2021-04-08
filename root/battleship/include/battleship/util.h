/*
 * util.h
 *
 *  Created on: 22 Oct 2018
 *      Author: pejdavies1606
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "battleship/commondefs.h"

void ReadString(char *str, size_t strlen, FILE *stream);
bool ParseUnsignedLong(const char *str, unsigned long *val);
int CalcMax(const int *data, uint n);
int CalcNumWidth(int n);
int RepeatChar(String_t line, size_t size_line, char elem);

#endif /* UTIL_H_ */
