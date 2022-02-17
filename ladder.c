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
  contact contact_a;
  contact_init(&contact_a, &b,NORMALLY_CLOSED);
  contact contact_b;
  contact_init(&contact_b, &b,NORMALLY_CLOSED);

  coil coil_a;
  coil_normal_init(&coil_a,&a);

  coil coil_b;
  coil_normal_init(&coil_b,&b);

  branch branch_a = { 0 };
  branch_init (&arena,&branch_a);

  rung myrung = { 0 };
  rung_init (&arena, &myrung);

  branch_add_element_up ((branch *) &branch_a,
                         (element *) &contact_a);
  branch_add_element_down ((branch *) &branch_a,
                           (element *) &contact_b);
  rung_add_element (&myrung, (element *) &branch_a);

  rung_add_element_copy (&arena,&myrung, (element *) &contact_a);
  rung_add_element_copy (&arena,&myrung, (element *) &contact_b);
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
