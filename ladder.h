#ifndef LADDER_H_
#define LADDER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "./utils.h"
#include "./memory.h"

enum
{
  CONTACT,
  COIL,
  BRANCH,
} element_type;

typedef s32 type;

typedef struct
{
  type elm_type;
  b32 *input;
  b32 output;
  s8 contact_type;
  b32 *variable;
  b32 variable_old;
} contact;

enum
{
  NORMALLY_OPEN,
  NORMALLY_CLOSED,
  /* TODO(accacio): implement positive and negative */
} contact_type;

void
contact_normallyopen_init (contact*_contact,b32*variable)
{

  _contact->variable = variable;
  _contact->elm_type = CONTACT;
  _contact->contact_type = NORMALLY_OPEN;
}

void
contact_normallyclosed_init (contact*_contact,b32*variable)
{
  _contact->variable = variable;
  _contact->elm_type = CONTACT;
  _contact->contact_type = NORMALLY_CLOSED;
}
void
contact_init (contact*_contact,b32*variable,type _type)
{
  _contact->variable = variable;
  _contact->elm_type = CONTACT;
  _contact->contact_type = _type;
}

void
contact_update (contact *_contact)
{
  switch (_contact->contact_type)
    {
    case NORMALLY_OPEN:
      _contact->variable_old = *(_contact->variable);
      _contact->output = _contact->variable_old && *(_contact->input);
      break;
    case NORMALLY_CLOSED:
      _contact->variable_old = *(_contact->variable);
      _contact->output = !_contact->variable_old && *(_contact->input);
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
  if (_contact->variable_old)
    variable_color = GREEN;
  if (_contact->output)
    output_color = GREEN;

  printf ("\e[38;5;%d;1m-|", input_color);
  printf ("\e[38;5;%d;1m", variable_color);
  switch (_contact->contact_type)
    {
    case NORMALLY_OPEN:
      printf ("▪");
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
coil_normal_init (coil *_coil, b32 *variable)
{
  _coil->variable = variable;
  _coil->elm_type = COIL;
  _coil->coil_type = COIL_NORMAL;
}

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
void element_list_add (element_list *, element *, b32 *);

typedef struct
{
  s32 elm_type;
  b32 *input;
  b32 output;
  element_list *branch_up;
  element_list *branch_down;
} branch;

void
branch_add_element_up (branch *_branch, element *_element)
{
  element_list_add (_branch->branch_up, _element, _branch->input);
}
void
branch_add_element_down (branch *_branch, element *_element)
{
  element_list_add (_branch->branch_down, _element, _branch->input);
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

void print_branch (branch *);
void branch_update (branch *);

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

void
element_update (element *_element)
{
  switch (_element->elm_type)
    {
    case COIL:
      coil_update ((coil *) _element);
      break;
    case CONTACT:
      contact_update ((contact *) _element);
      break;
    case BRANCH:
      branch_update ((branch *) _element);
      break;
    default:
      break;
    }
}

typedef struct _element_list
{
  element **data;
  s32 count;
  s32 max;
} element_list;

void
element_list_init (Arena *arena, element_list *list)
{
  list->max = 10;
  list->count = 0;
  list->data = push_array (arena, list->max, element *);
}

void
element_list_add_copy (Arena *arena, element_list *list, element *_element,
                       b32 *initial)
{
  element **cur_element = &list->data[list->count];
  *cur_element = push_struct (arena, element);
  **cur_element = *_element;
  if (list->count == 0)
    {
      (*cur_element)->input = (initial);
    }
  else
    {
      (*cur_element)->input = &((list->data[list->count - 1])->output);
    }

  list->count += 1;
}
void
element_list_add (element_list *list, element *_element, b32 *initial)
{
  element **cur_element = &list->data[list->count];
  *cur_element = _element;
  if (list->count == 0)
    {
      (*cur_element)->input = (initial);
    }
  else
    {
      (*cur_element)->input = &((list->data[list->count - 1])->output);
    }

  list->count += 1;
}

void
element_list_destroy (element_list *list)
{
  /* free (list->data); */
}

typedef struct
{
  element_list *elements;
  b32 rail;
} rung;

void
rung_init (Arena *arena, rung *_rung)
{
  _rung->rail = 1;

  _rung->elements = push_struct (arena, element_list);
  element_list_init (arena, _rung->elements);
}
void
rung_destroy (rung *_rung)
{
  element_list_destroy (_rung->elements);
  /* free (_rung->elements); */
}

void
rung_add_element_copy (Arena *arena, rung *_rung, element *_element)
{
  element_list_add_copy (arena, _rung->elements, _element, &_rung->rail);
}
void
rung_add_element (rung *_rung, element *_element)
{
  element_list_add (_rung->elements, _element, &_rung->rail);
}

void
rung_update (rung *_rung)
{
  for (int i = 0; i < _rung->elements->count; i++)
    {
      element_update ((_rung->elements->data[i]));
    }
}
void
print_rung (rung *_rung)
{
  for (int i = 0; i < _rung->elements->count; i++)
    {
      print_element (*(_rung->elements->data + i));
    }
  printf ("\n");
}

void
branch_init (Arena *arena, branch *_branch)
{
  _branch->elm_type = BRANCH;
  _branch->input = (b32*)_branch;
  _branch->branch_up = push_struct (arena, element_list);
  element_list_init (arena, _branch->branch_up);

  _branch->branch_down = push_struct (arena, element_list);
  element_list_init (arena, _branch->branch_down);
}

void
branch_update (branch *_branch)
{
  s32 up_count = _branch->branch_up->count;
  s32 down_count = _branch->branch_down->count;
  for (int i = 0; i < up_count; i++)
    {
      element_update ((_branch->branch_up->data[i]));
    }

  for (int i = 0; i < down_count; i++)
    {
      element_update ((_branch->branch_down->data[i]));
    }
  if (up_count == 0 || down_count == 0)
    {
      _branch->output = *_branch->input;
    }
  else
    {
      _branch->output = _branch->branch_up->data[up_count - 1]->output
                        || _branch->branch_down->data[down_count - 1]->output;
    }
}

void
print_branch (branch *_branch)
{
  s32 input_color = RED;
  s32 output_color = RED;
  s32 variable_color = RED;
  if (*_branch->input)
    input_color = GREEN;
  if (_branch->output)
    output_color = GREEN;

  printf ("\e[38;5;%d;1m-/\e[0m", input_color);
  printf (" BRANCH UP ");

  for (int i = 0; i < _branch->branch_up->count; i++)
    {
      print_element (*(_branch->branch_up->data + i));
    }
  printf ("\e[38;5;%d;1m-/\e[0m", output_color);
  printf ("\e[38;5;%d;1m-\\\e[0m", input_color);
  printf ("BRANCH DOWN");
  for (int i = 0; i < _branch->branch_down->count; i++)
    {
      print_element (*(_branch->branch_down->data + i));
    }
  printf ("\e[38;5;%d;1m-\\\e[0m", output_color);
}

#endif // LADDER_H_
