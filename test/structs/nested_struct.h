#include "simple_struct.h"

#ifndef _NEST_H
#define _NEST_H

typedef struct simple_nest
{
  unsigned char  u8;
  unsigned short u16;
  unsigned int   u32;
  short          i16;
  int            i32;
} simple_nest_t;

typedef struct nest
{
  unsigned char u8;
  simple_nest_t simple;
} nest_t;

#endif