/*
 * form_curses.c
 *
 *  Created on: 12 September 2022
 *      Author: pejdavies1606
 */

#include "battleship/form.h"
#include "battleship/form_curses.h"

bool Form_InitData(Form_t *const form)
{
   bool result = false;
   if (form)
   {
      if (form->num_fields > 0U)
      {
         FIELD **c_fields = calloc(form->num_fields * 2U + 1U, sizeof(FIELD *));
         if (c_fields)
         {
            result = true;
            for (uint i = 0U; i < form->num_fields * 2U; i+=2)
            {
               Field_t *field = &form->fields[i];
               size_t prompt_len = strlen(field->prompt);
               for (uint j = i; j < i + 2U; j++)
               {
                  if (j % 2U == 0)
                  {
                     // prompt
                     c_fields[j] = new_field(
                         1, (int)prompt_len,
                         (int)i + 1, 0,
                         0, 0);
                  }
                  else
                  {
                     // value
                     c_fields[i + 1] = new_field(
                         1, (int)field->len,
                         (int)i + 1, (int)prompt_len + 1,
                         0, 0);
                  }
                  if (!c_fields[j])
                  {
                     result = false;
                     break;
                  }
                  if (j % 2U == 0)
                  {
                     set_field_buffer(c_fields[j], 0, form->fields[i].prompt);
                     field_opts_off(c_fields[j], O_ACTIVE);
                  }
                  else
                  {
                     set_field_back(c_fields[j], A_UNDERLINE);
                     field_opts_off(c_fields[j], O_AUTOSKIP);
                  }
               }
            }
         }
         if (result)
         {
            FORM *c_form = new_form(c_fields);
            if (!c_form)
            {
               result = false;
            }
            else
            {
               FormData_t *data = malloc(sizeof(FormData_t));
               if (!data)
               {
                  result = false;
               }
               else
               {
                  data->fields = c_fields;
                  data->form = c_form;
                  form->data = (void *)data;
               }
            }
         }
         if (!result)
         {
            Form_DestroyData(form);
         }
      }
   }
   return result;
}

void Form_DestroyData(Form_t *const form)
{
   if (form)
   {
      FormData_t *data = form->data;
      if (data)
      {
         for (uint i = 0U; i < form->num_fields * 2U; i++)
         {
            free_field(data->fields[i]);
         }
         free_form(data->form);
         free(data);
      }
   }
}