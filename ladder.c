#include <stdio.h>
#include <assert.h>

#include "./utils.h"
#include "./ladder.h"

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
