#ifndef _SIMPLE_H
#define _SIMPLE_H

typedef char int8_t;

typedef struct simple
{
  unsigned char  u8;
  unsigned short u16;
  unsigned int   u32;
  int8_t         i8;
  short          i16;
  int            i32;
  char           c;
  float          f32;
  double         f64;
  unsigned char  u8check;
} simple_t;

typedef struct simple_not_used
{
  unsigned char u8;
} simple_not_used_t;

#endif