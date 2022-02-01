#ifndef LADDER_H_
#define LADDER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./utils.h"

typedef struct
{
  s32 elm_type;
  b32 *input;
  b32 output;
  s8 contact_type;
  b32 *variable;
} contact;

enum
{
  NORMALLY_OPEN,
  NORMALLY_CLOSED,
  /* TODO(accacio): implement positive and negative */
} contact_type;

void
contact_update (contact *_contact)
{
  switch (_contact->contact_type)
    {
    case NORMALLY_OPEN:
      _contact->output = *(_contact->variable) && *(_contact->input);
      break;
    case NORMALLY_CLOSED:
      _contact->output = !(*(_contact->variable)) && *(_contact->input);
      break;
    default:
      break;
    }
}

#define RED 1
#define GREEN 2

void
print_contact (contact *_contact)
{
  s32 input_color = RED;
  s32 output_color = RED;
  s32 variable_color = RED;
  if (*_contact->input)
    input_color = GREEN;
  if (*_contact->variable)
    variable_color = GREEN;
  if (_contact->output)
    output_color = GREEN;

  printf ("\e[38;5;%d;1m-|", input_color);
  printf ("\e[38;5;%d;1m", variable_color);
  switch (_contact->contact_type)
    {
    case NORMALLY_OPEN:
      printf (" ");
      break;
    case NORMALLY_CLOSED:
      printf ("\\");
      break;
    default:
      break;
    }
  printf ("\e[38;5;%d;1m|-\e[0m", output_color);
}

typedef struct
{
  s32 elm_type;
  b32 *input;
  b32 output;
  s8 coil_type;
  b32 *variable;
} coil;

enum
{
  COIL_NORMAL,
  /* TODO(accacio): implement set and reset */
} coil_type;

void
coil_update (coil *_coil)
{
  switch (_coil->coil_type)
    {
    case COIL_NORMAL:
      {
        _coil->output = *(_coil->input);
        *(_coil->variable) = *(_coil->input);
      }
      break;
    default:
      break;
    }
}

void
print_coil (coil *_coil)
{
  s32 input_color = RED;
  s32 output_color = RED;
  s32 variable_color = RED;
  if (*_coil->input)
    input_color = GREEN;
  if (*_coil->variable)
    variable_color = GREEN;
  if (_coil->output)
    output_color = GREEN;

  printf ("\e[38;5;%d;1m-", input_color);
  printf ("\e[38;5;%d;1m", variable_color);
  switch (_coil->coil_type)
    {
    case COIL_NORMAL:
      printf ("●");
      break;
    default:
      break;
    }
  printf ("\e[38;5;%d;1m-\e[0m", output_color);
}

typedef union _element element;
typedef struct _element_list element_list;

typedef struct
{
  s32 elm_type;
  b32 *input;
  b32 output;
  element_list *branch_up;
  element_list *branch_down;
} branch;

void
print_branch (branch *_branch)
{
  printf ("STUB\n");
}

typedef union _element
{
  struct
  {
    s32 elm_type;
    b32 *input;
    b32 output;
  };
  coil elm_coil;
  contact elm_contact;
  branch elm_branch;
} element;

enum
{
  CONTACT,
  COIL,
  BRANCH,
} element_type;

void
print_element (element *_element)
{
  switch (_element->elm_type)
    {
    case COIL:
      print_coil ((coil *) _element);
      break;
    case CONTACT:
      print_contact ((contact *) _element);
      break;
    case BRANCH:
      print_branch ((branch *) _element);
      break;
    default:
      break;
    }
}

typedef struct _element_list
{
  element *data;
  s32 count;
  s32 max;
} element_list;

void
element_list_init (element_list *list)
{
  list->max = 10;
  list->count = 0;
  list->data = malloc (list->max * sizeof (element));
  memset (list->data, 0, list->max * sizeof (element));
}

void
element_list_add (element_list *list, element *_element, b32 *initial)
{
  element *cur_element = &list->data[list->count];
  *cur_element = *_element;
  if (list->count == 0)
    {
      cur_element->input = (initial);
    }
  else
    {
      cur_element->input = &(list->data[list->count - 1]).output;
    }

  list->count += 1;
}

void
element_list_destroy (element_list *list)
{
  free (list->data);
}

typedef struct
{
  element_list *elements;
  b32 rail;
} rung;

void
rung_init (rung *_rung)
{
  _rung->rail = 1;
  _rung->elements = malloc (sizeof (element_list));
  memset (_rung->elements, 0, sizeof (element_list));
  element_list_init (_rung->elements);
}
void
rung_destroy (rung *_rung)
{
  element_list_destroy (_rung->elements);
  free (_rung->elements);
}

void
rung_add_element (rung *_rung, element *_element)
{
  element_list_add(_rung->elements, _element, &_rung->rail);
}

void
rung_update (rung *_rung)
{
  for (int i = 0; i < _rung->elements->count; i++)
    {
      contact *cur_element = (contact *) &(_rung->elements->data[i]);
      contact_update (cur_element);
    }
  printf ("\n");
}
void
print_rung (rung *_rung)
{
  for (int i = 0; i < _rung->elements->count; i++)
    {
      print_element (_rung->elements->data + i);
    }
  printf ("\n");
}

#endif // LADDER_H_
