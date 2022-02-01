#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef uint32_t b32;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef struct
{
  s32 elm_type;
  s8 contact_type;
  b32 *variable;
  b32 *input;
  b32 output;
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
  s8 coil_type;
  b32 *variable;
  b32 *input;
  b32 output;
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

typedef union
{
  s32 elm_type;
  coil elm_coil;
  contact elm_contact;
} element;

enum
{
  CONTACT,
  COIL,
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
  memset(list->data, 0, list->max*sizeof(element));
}
void
element_list_destroy (element_list *list)
{
  free(list->data);
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
  _rung->elements = malloc(sizeof(element_list));
  memset(_rung->elements,0,sizeof(element_list));
  element_list_init(_rung->elements);
}
void
rung_destroy (rung *_rung)
{
  element_list_destroy(_rung->elements);
  free(_rung->elements);
}

void
rung_add_contact (rung *_rung, contact *_contact)
{
  assert (_rung->elements->count < _rung->elements->max);
  contact *cur_element = (contact *) &(_rung->elements->data[_rung->elements->count]);
  *cur_element = *_contact;
  if (_rung->elements->count == 0)
    {
      cur_element->input = &(_rung->rail);
    }
  else
    {
      cur_element->input = &(
          ((contact *) (_rung->elements + (_rung->elements->count - 1)))->output);
    }
  _rung->elements->count += 1;
}
void
rung_add_coil (rung *_rung, coil *_coil)
{
  assert (_rung->elements->count < _rung->elements->max);
  coil *cur_element = (coil *) &(_rung->elements->data[_rung->elements->count]);
  *cur_element = *_coil;
  if (_rung->elements->count == 0)
    {
      cur_element->input = &(_rung->rail);
    }
  else
    {
      cur_element->input
          = &(((coil *) _rung->elements + (_rung->elements->count - 1))->output);
    }
  _rung->elements->count += 1;
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

int
main (int argc, char *argv[])
{
  contact contact_a = { 0 };
  contact contact_b = { 0 };
  b32 a = 0;
  b32 b = 0;
  contact_a.variable = &a;
  contact_a.elm_type = CONTACT;
  contact_a.contact_type = NORMALLY_CLOSED;
  contact_b.variable = &b;
  contact_b.contact_type = NORMALLY_OPEN;

  coil coil_a = { 0 };
  coil_a.variable = &a;
  coil_a.elm_type = COIL;
  coil_a.coil_type = COIL_NORMAL;

  rung myrung = { 0 };
  rung_init (&myrung);
  /* contact_a.input = &myrung.rail; */
  rung_add_contact (&myrung, &contact_a);
  rung_add_contact (&myrung, &contact_b);
  rung_add_coil (&myrung, &coil_a);

  rung_update (&myrung);
  print_rung (&myrung);
  a = 1;
  rung_update (&myrung);
  print_rung (&myrung);
  a = 0;
  b=1;
  rung_update (&myrung);
  print_rung (&myrung);

  rung_destroy(&myrung);
  /* myrung.elements; */
  /* printf ("%d\n", (myrung.elements)->output); */
  return 0;
}
