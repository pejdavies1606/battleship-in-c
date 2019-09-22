/*
 * array_util.h
 *
 *  Created on: 20 Oct 2018
 *      Author: pejdavies1606
 */

#ifndef ARRAY_UTIL_H_
#define ARRAY_UTIL_H_

#include <stdlib.h>

// https://stackoverflow.com/questions/11932428/how-to-append-two-arrays-in-c-language
#define ARRAY_CONCAT(TYPE, A, An, B, Bn) \
(TYPE *)array_concat((const void *)(A), (An), (const void *)(B), (Bn), sizeof(TYPE));

void *array_concat(const void *a, size_t an,
                   const void *b, size_t bn, size_t s);
#endif /* ARRAY_UTIL_H_ */
