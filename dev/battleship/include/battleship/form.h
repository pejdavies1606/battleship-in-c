/*
 * form.h
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#ifndef FORM_H_
#define FORM_H_

#include "battleship/commondefs.h"
#include "battleship/input.h"

typedef struct Field
{
   char *prompt;
   uint len;
   InputData_t *data;
} Field_t;

typedef struct Form
{
   char *title;
   uint num_fields;
   Field_t *fields;
   void *data; // implementation specific
} Form_t;

bool Form_InitData(Form_t *const form);
void Form_DestroyData(Form_t *const form);

#endif /* FORM_H_ */