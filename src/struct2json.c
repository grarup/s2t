#include "struct2json.h"
#include "string.h"
#include "json-maker.h"

char * json_member(char * target, unsigned int * targetMaxLength, char * name, unsigned char * data, types_t type, void * child)
{
  switch (type & types_typeMask)
  {
    case types_u8:
      target = json_uint(target, name, *data, targetMaxLength);
      break;
    case types_u16:
      target = json_uint(target, name, *((unsigned short *)data), targetMaxLength);
      break;
    case types_u32:
      target = json_uint(target, name, *((unsigned int *)data), targetMaxLength);
      break;
    case types_i8:
      target = json_int(target, name, *((char *)data), targetMaxLength);
      break;
    case types_i16:
      target = json_int(target, name, *((short *)data), targetMaxLength);
      break;
    case types_i32:
      target = json_int(target, name, *((int *)data), targetMaxLength);
      break;
    case types_sz:
      target = json_str(target, name, data, targetMaxLength);
      break;
    case types_struct:
      target = json_struct(target, targetMaxLength, data, child);
      break;
    default:
      break;
  }
  return target;
}

static char * json_array(char * target, unsigned int * targetMaxLength, char * name, unsigned char * arrayPointer, int count, types_t type, void * child)
{
  target = json_arrOpen(target, name, targetMaxLength);
  for (int i = 0; i < count; i++)
  {
    printf("arrayPoint %x, %i\n", arrayPointer, type);
    target = json_member(target, targetMaxLength, NULL, arrayPointer, type, child);
    switch (type)
    {
      case types_i8:
      case types_u8:
        arrayPointer = (unsigned char *)(arrayPointer + 1);
        break;
      case types_i16:
      case types_u16:
        arrayPointer = (unsigned char *)(arrayPointer + 2);
        break;
      case types_i32:
      case types_u32:
      case types_sz:
        arrayPointer = (unsigned char *)(arrayPointer + 4);
        break;

      default:
        break;
    }
  }
  target = json_arrClose(target, targetMaxLength);
  return target;
}

char * json_multi_array(char * target, unsigned int * targetMaxLength, char * name, unsigned char * arrayPointer, int count, types_t type, void * child, unsigned int * arrayDim)
{
  printf("type %i\n", type);
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
  for (unsigned int i = 0; i < structBody->count; i++)
  {
    structMember_t * member = (structMember_t *)&structBody->members[i];
    unsigned char *  data   = (unsigned char *)(basePointer + member->offset);
    if (member->type & types_multiArray)
    {
      target = json_multi_array(target, targetMaxLength, member->name, data, member->count, member->type & types_typeMask, NULL, member->child);
    }
    else if (member->count > 0)
    {
      target = json_array(target, targetMaxLength, member->name, data, member->count, member->type, member->child);
    }
    else
    {
      if (member->type & types_pointer)
      {
        if ((unsigned char *)*((unsigned int *)data) == NULL)
        {
          target = json_null(target, member->name, targetMaxLength);
          continue;
        }
        data = (unsigned char *)*((unsigned int *)data);
      }
      target = json_member(target, targetMaxLength, member->name, data, member->type, member->child);
    }
  }
  target = json_objClose(target, targetMaxLength);
}