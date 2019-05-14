/*
 * util.h
 *
 *  Created on: 22 Oct 2018
 *      Author: pejdavies1606
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "commondefs.h"

#define strlens(s) IF_NULL(s,0,strlen(s))

void ReadString(char *str, int strlen, FILE *stream);
bool ParseUnsignedLong(const char *str, unsigned long *val);
uint Number_Width(int n);

#endif /* UTIL_H_ */
