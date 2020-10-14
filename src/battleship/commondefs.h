/*
 * commondefs.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef COMMONDEFS_H_
#define COMMONDEFS_H_

#include <stdint.h>
#include <stdlib.h> // NULL, size_t
#include <stdbool.h> // bool

#ifndef NDEBUG
#include <stdio.h> // puts, printf
#endif

#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))

#define IF_NULL(a,b,c) (( NULL == a ) ? b : c)
#define IF_NULL_VAL(a,b) (( NULL == a ) ? b : a)
#define IF_NULL_BLANK(s) ( (NULL == s) ? "" : s)
#define strlens(s) IF_NULL(s,0,strlen(s))

typedef unsigned int uint;
typedef char *String_t;

#endif /* COMMONDEFS_H_ */
