#ifndef _ARRAY_STRUCT_H
#define _ARRAY_STRUCT_H

typedef struct array
{
  unsigned char  u8notarray;
  unsigned char  u8[3];
  unsigned short u16[5];
  unsigned int   u32[2];
  unsigned int   m_u32[3][2];
  char           string[32];
} array_t;

#endif