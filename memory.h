#ifndef MEMORY_H_
#define MEMORY_H_

#include <stdlib.h>
#include <stdio.h>
#include "./utils.h"

typedef struct
{
  size_t size;
  u8 *base;
  size_t used;
} Arena;

void
initialize_arena (Arena *arena, size_t size, u8 *base)
{
  arena->size = size;
  arena->base = base;
  arena->used = 0;
}

#define push_struct(arena, type) (type *) push_struct_ (arena, sizeof (type))
#define push_array(arena, count, type) (type *) push_struct_ (arena, (count)*sizeof (type))
void *
push_struct_ (Arena *arena, size_t size)
{
  Assert ((arena->used + size) <= arena->size);
  void *result = arena->base + arena->used;
  arena->used += size;
  return result;
}


#endif // MEMORY_H_
