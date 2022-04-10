#include "struct2text.h"

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