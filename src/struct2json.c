#include "struct2json.h"
#include "string.h"
#include "json-maker.h"

char * json_member(char * target, unsigned int * targetMaxLength, char * name, unsigned char * data, types_t type, void * child)
{
  unsigned char * valuePointer = data;
  if (type & types_pointer)
  {
    if ((unsigned char *)*((unsigned int *)valuePointer) == NULL)
    {
      type = types_pointer;
    }
    valuePointer = (unsigned char *)*((unsigned int *)valuePointer);
  }

  switch (type & types_typeMask)
  {
    case types_u8:
      target = json_uint(target, name, *valuePointer, targetMaxLength);
      break;
    case types_u16:
      target = json_uint(target, name, *((unsigned short *)valuePointer), targetMaxLength);
      break;
    case types_u32:
      target = json_uint(target, name, *((unsigned int *)valuePointer), targetMaxLength);
      break;
    case types_i8:
      target = json_int(target, name, *((char *)valuePointer), targetMaxLength);
      break;
    case types_i16:
      target = json_int(target, name, *((short *)valuePointer), targetMaxLength);
      break;
    case types_i32:
      target = json_int(target, name, *((int *)valuePointer), targetMaxLength);
      break;
    case types_f32:
      target = json_double(target, name, *((float *)valuePointer), targetMaxLength);
      break;
    case types_f64:
      target = json_double(target, name, *((double *)valuePointer), targetMaxLength);
      break;
    case types_char:
      target = json_nstr(target, name, valuePointer, 1, targetMaxLength);
      break;
    case types_sz:
      target = json_str(target, name, valuePointer, targetMaxLength);
      break;
    case types_struct:
      target = json_struct(target, targetMaxLength, valuePointer, child);
      break;
    default:
      target = json_null(target, name, targetMaxLength);
      break;
  }
  return target;
}

static char * json_array(char * target, unsigned int * targetMaxLength, char * name, unsigned char ** data, int count, types_t type, void * child)
{
  if (type == types_char)
  {
    target = json_nstr(target, name, *data, count, targetMaxLength);
    *data  = (unsigned char *)(*data + count);
    return target;
  }
  target = json_arrOpen(target, name, targetMaxLength);
  for (int i = 0; i < count; i++)
  {
    target            = json_member(target, targetMaxLength, NULL, *data, type, child);
    unsigned int size = getSize(type, child);
    *data             = (unsigned char *)(*data + size);
  }
  target = json_arrClose(target, targetMaxLength);
  return target;
}

char * json_multi_array(char * target, unsigned int * targetMaxLength, char * name, unsigned char ** arrayPointer, int count, types_t type, void * child, unsigned int * arrayDim)
{
  if (count == 1)
  {
    target = json_array(target, targetMaxLength, NULL, arrayPointer, *arrayDim, type, child);
  }
  else
  {
    target = json_arrOpen(target, name, targetMaxLength);
    for (size_t i = 0; i < *arrayDim; i++)
    {
      target = json_multi_array(target, targetMaxLength, NULL, arrayPointer, count - 1, type, child, arrayDim + 1);
    }

    target = json_arrClose(target, targetMaxLength);
  }
  return target;
}

char * json_struct(char * target, unsigned int * targetMaxLength, unsigned char * structPointer, structBody_t * structBody)
{
  unsigned char * basePointer = structPointer;
  target                      = json_objOpen(target, structBody->name, targetMaxLength);
  unsigned char * data        = structPointer;
  for (unsigned int i = 0; i < structBody->count; i++)
  {
    structMember_t * member = (structMember_t *)&structBody->members[i];
    unsigned char *  data   = (unsigned char *)(basePointer + member->offset);
    if (member->type & types_multiArray)
    {
      target = json_multi_array(target, targetMaxLength, member->name, &data, member->multi->count, member->type & types_typeMask, NULL, member->multi->lengths);
    }
    else if (member->count > 0)
    {
      target = json_array(target, targetMaxLength, member->name, &data, member->count, member->type, member->child);
    }
    else
    {
      target = json_member(target, targetMaxLength, member->name, data, member->type, member->child);
    }
  }
  target = json_objClose(target, targetMaxLength);
}