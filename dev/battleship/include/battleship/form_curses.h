/*
 * form_curses.h
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#ifndef FORM_CURSES_H_
#define FORM_CURSES_H_

#include <form.h>

typedef struct FormData
{
   FIELD **fields;
   FORM *form;
} FormData_t;

#endif /* FORM_CURSES_H_ */
