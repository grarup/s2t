
#ifndef _STRUCT_TO_TEXT_H
#define _STRUCT_TO_TEXT_H

typedef enum types
{
  types_null,
  types_u8,
  types_u16,
  types_u32,
  types_i8,
  types_i16,
  types_i32,
  types_sz,
  types_struct,
  types_typeMask   = 0x3FFFFFFF,
  types_multiArray = 0x40000000,
  types_pointer    = 0x80000000
} types_t;

typedef struct structMulitArray
{
  unsigned int   count;
  unsigned int * lengths;
} structMulitArray_t;

struct structBody;

typedef struct structMember
{
  char *       name;
  types_t      type;
  unsigned int offset;
  union
  {
    unsigned int         count;
    structMulitArray_t * multi;
  };
  struct structBody * child;
} structMember_t;

typedef struct structBody
{
  char *                 name;
  unsigned int           count;
  unsigned int           size;
  const structMember_t * members;
} structBody_t;

unsigned int getSize(types_t type, structBody_t * child);

#endif