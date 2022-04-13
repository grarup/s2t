#include "struct2text.h"
#include "stddef.h"
#include "string.h"

unsigned int getSize(types_t type, structBody_t * child)
{
  if (type & types_pointer)
    return 4;
  switch (type & types_typeMask)
  {
    case types_i8:
    case types_u8:
      return 1;
    case types_i16:
    case types_u16:
      return 2;
    case types_i32:
    case types_u32:
    case types_f32:
      return 4;
    case types_f64:
      return 8;
    case types_struct:
      return child->size;
    default:
      break;
  }
  return 0;
}

const structMember_t * getMember(structBody_t * body, char * name)
{
  for (unsigned int i = 0; i < body->count; i++)
  {
    if (strcmp(name, body->members[i].name) == 0)
      return &body->members[i];
  }
  return NULL;
}
