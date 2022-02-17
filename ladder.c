#include <stdio.h>
#include <assert.h>

#include "./utils.h"
#include "./ladder.h"

int
main (int argc, char *argv[])
{
  /* NOTE(accacio): all memory allocated for the program */
  size_t memory_size = Kilobytes (20);
  u8 *memory = malloc (memory_size);
  Arena arena;
  initialize_arena (&arena, memory_size, memory);

  b32 a;
  b32 b;
  contact contact_a = { 0 };
  contact contact_b = { 0 };
  contact_a.variable = &a;
  contact_a.elm_type = CONTACT;
  contact_a.contact_type = NORMALLY_CLOSED;
  contact_b.variable = &b;
  contact_b.contact_type = NORMALLY_CLOSED;

  coil coil_a = { 0 };
  coil_a.variable = &a;
  coil_a.elm_type = COIL;
  coil_a.coil_type = COIL_NORMAL;

  coil coil_b = { 0 };
  coil_b.variable = &b;
  coil_b.elm_type = COIL;
  coil_b.coil_type = COIL_NORMAL;

  /* return 0; */

  branch branch_a = { 0 };
  branch_a.elm_type = BRANCH;
  branch_init (&arena,&branch_a);

  rung myrung = { 0 };
  rung_init (&arena, &myrung);
  contact_a.input = &myrung.rail;
  rung_add_element (&myrung, (element *) &branch_a);

  branch_add_element_up ((branch *) &myrung.elements->data[0],
                         (element *) &contact_a);
  branch_add_element_down ((branch *) &myrung.elements->data[0],
                           (element *) &contact_b);

  rung_add_element (&myrung, (element *) &contact_a);
  rung_add_element (&myrung, (element *) &contact_b);
  rung_add_element (&myrung, (element *) &coil_a);
  rung_add_element (&myrung, (element *) &coil_b);

  a = 0;
  b = 1;
  printf ("%d\n", a);
  printf ("%d\n", b);
  rung_update (&myrung);
  print_rung (&myrung);
  printf ("%d\n", a);
  printf ("%d\n", b);

  /* a = 1; */
  /* b = 0; */
  rung_update (&myrung);
  print_rung (&myrung);
  printf ("%d\n", a);
  printf ("%d\n", b);
  /* a = 0; */
  /* b = 1; */
  rung_update (&myrung);
  print_rung (&myrung);
  printf ("%d\n", a);
  printf ("%d\n", b);

  /* a = 1; */
  /* b = 1; */
  rung_update (&myrung);
  print_rung (&myrung);
  printf ("%d\n", a);
  printf ("%d\n", b);

  rung_destroy (&myrung);
  /* myrung.elements; */
  /* printf ("%d\n", (myrung.elements)->output); */

  free (memory);

  return 0;
}
