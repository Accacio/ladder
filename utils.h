#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#define Assert(exp)                                                           \
  if (!(exp))                                                                 \
    {                                                                         \
      printf ("%s:%d: Assert \"%s\" failed\n", __FILE__, __LINE__, #exp);     \
      *(volatile int *) 0 = 0;                                                \
    }

#define Kilobytes(num) (num)*1024LL
#define Megabytes(num) Kilobytes((num))*1024LL

typedef uint32_t b32;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;


#endif // UTILS_H_
