/*
 * util.h
 *
 *  Created on: 22 Oct 2018
 *      Author: pejdavies1606
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "battleship/commondefs.h"

char * TrimStr(char * str, size_t str_size);
int CalcMax(const int *data, uint n);
int CalcNumWidth(int n);
bool RepeatChar(char * line, size_t size_line, char elem);

#endif /* UTIL_H_ */
