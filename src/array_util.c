/*
 * array_util.c
 *
 *  Created on: 20 Oct 2018
 *      Author: pejdavies1606
 */

#include "array_util.h"

#include <string.h>

void *array_concat(const void *a, size_t an,
                   const void *b, size_t bn, size_t s)
{
    char *p = malloc(s * (an + bn));
    memcpy(p, a, an*s);
    memcpy(p + an*s, b, bn*s);
    return p;
}
