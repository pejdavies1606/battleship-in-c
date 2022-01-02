/*
 * commondefs.h
 *
 *  Created on: 2 Jul 2018
 *      Author: pejdavies1606
 */

#ifndef COMMONDEFS_H_
#define COMMONDEFS_H_

#include <stdlib.h> // NULL, size_t, uint
#include <stdbool.h> // bool

#define IF_NULL(a,b,c) (( NULL == a ) ? b : c)
#define IF_NULL_VAL(a,b) (( NULL == a ) ? b : a)
#define IF_NULL_BLANK(s) ( (NULL == s) ? "" : s)

#define UNUSED(x) (void)(x)

#ifndef NDEBUG
#define DEBUG 1
#include <stdio.h> // puts, printf
#else
#define DEBUG 0
#endif

#define debug_print(fmt, ...) \
      do { \
         if (DEBUG) \
            fprintf(stderr, "%s: " fmt, \
                  __func__, __VA_ARGS__); \
      } while (0)

#endif /* COMMONDEFS_H_ */
