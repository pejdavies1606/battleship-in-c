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

#define DEBUG 1

#ifdef DEBUG
#include <stdio.h> // puts, printf
#endif

typedef unsigned int uint;
typedef char *String_t;

#endif /* COMMONDEFS_H_ */
