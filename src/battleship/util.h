/*
 * util.h
 *
 *  Created on: 22 Oct 2018
 *      Author: pejdavies1606
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "commondefs.h"

#define IF_NULL_VAL(a,b) (( NULL == a ) ? b : a)
#define IF_NULL(a,b,c) (( NULL == a ) ? b : c)
#define strlens(s) IF_NULL(s,0,strlen(s))

void ReadString(char *str, int strlen, FILE *stream);
bool ParseUnsignedLong(const char *str, unsigned long *val);
int CalcMax(const int *data, uint n);
int CalcNumWidth(int n);
int RepeatChar(String_t line, size_t size_line, char elem);

#endif /* UTIL_H_ */
