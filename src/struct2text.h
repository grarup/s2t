
#ifndef _STRUCT_TO_TEXT_H
#define _STRUCT_TO_TEXT_H

typedef enum types
{
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

typedef struct structMember
{
  char *       name;
  types_t      type;
  unsigned int offset;
  unsigned int count;
  void *       child;
} structMember_t;

typedef struct structBody
{
  char *                 name;
  int                    count;
  const structMember_t * members;
} structBody_t;

char * copyStr(char * target, char * source, unsigned int * maxLength);

#endif