#include "json2struct.h"
#include "string.h"
#include "stdlib.h"

static char * move_past(char * source, unsigned int * maxLength, char c)
{
  while (maxLength > 0 && *source != c)
  {
    source++;
    *maxLength--;
  }
  *maxLength--;
  return source + 1;
}

static inline char * read_start(char * source, unsigned int * maxLength)
{
  return move_past(source, maxLength, '{');
}

static inline char * move_past_divider(char * source, unsigned int * maxLength)
{
  return move_past(source, maxLength, ':');
}

static char * move_past_end(char * source, unsigned int * maxLength)
{
  int braced_count = 0;
  while (*maxLength > 1 && (braced_count > 0 || !(*source == ',' || *source == '}')))
  {
    if (*source == '{')
      braced_count++;
    else if (*source == '}')
      braced_count--;
    source++;
    *maxLength--;
  }
  *maxLength--;
  return source + 1;
}

static char * strnchr(char * src, unsigned int maxLength, char c)
{
  return strchr(src, c);
}

static unsigned char * get_string(unsigned char * source, unsigned int * maxLenght, char ** string)
{
  // TOOD make i handle escaped "
  *string = strnchr(source, *maxLenght, '"');
  if (*string != NULL)
  {
    *string += 1;
    *maxLenght -= ((unsigned int)source - (unsigned int)*string);
    char * end = strnchr(*string, *maxLenght, '"');
    *end       = 0;
    *maxLenght -= (*string - end) + 1;
    return end + 1;
  }
  return source;
}

static unsigned char * get_value_string(unsigned char * source, unsigned int * maxLenght, char ** string)
{
  char * end = source;
  while (*maxLenght > 0 && !(*end == ',' || *end == '}' || *end == ']'))
  {
    *maxLenght--;
    end++;
  }
  *string = source;
  *end    = 0;
  return end + 1;
}

static unsigned char * read_value(unsigned char * source, unsigned int * maxLength, unsigned char * structPointer, types_t type, structBody_t * child, int count)
{
  char * szValue;
  // printf("type %i\n", type);

  if (type & types_pointer)
  {
    if ((unsigned char *)*((unsigned int *)structPointer) == NULL)
    {
      return move_past_end(source, maxLength);
    }
    structPointer = (unsigned char *)*((unsigned int *)structPointer);
  }
  switch (type & types_typeMask)
  {
    case types_u8:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((unsigned char *)structPointer) = (unsigned char)atoi(szValue);
      }
      break;
    case types_u16:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((unsigned short *)structPointer) = (unsigned short)atoi(szValue);
      }
      break;
    case types_u32:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((unsigned int *)structPointer) = (unsigned int)atoi(szValue);
      }
      break;
    case types_i8:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((char *)structPointer) = (char)atoi(szValue);
      }
      break;
    case types_i16:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((short *)structPointer) = (short)atoi(szValue);
      }
      break;
    case types_i32:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((int *)structPointer) = atoi(szValue);
      }
      break;
    case types_f32:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((float *)structPointer) = atof(szValue);
      }
      break;
    case types_f64:
      source = get_value_string(source, maxLength, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((double *)structPointer) = strtod(szValue, NULL);
      }
      break;
    case types_char:
      source = get_string(source, maxLength, &szValue);
      source = move_past_end(source, maxLength);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((char *)structPointer) = szValue[0];
      }
      break;
    case types_sz:
      source = get_string(source, maxLength, &szValue);
      source = move_past_end(source, maxLength);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
        {
          if (count > 0)
          {
            strncpy((char *)structPointer, szValue, count);
            ((char *)structPointer)[count - 1] = 0;
          }
          else
            strcpy((char *)structPointer, szValue);
        }
      }
      break;
    case types_struct:
      source = read_struct_from_json(source, maxLength, structPointer, child);
      break;
    default:
      break;
  }
  return source;
}

static unsigned char * read_array(unsigned char * source, unsigned int * maxLength, unsigned char ** structPointer, unsigned int count, types_t type, structBody_t * child)
{
  if ((type & types_typeMask) == types_char)
  {
    source = read_value(source, maxLength, *structPointer, types_sz, NULL, count);
  }
  else
  {
    unsigned int size = getSize(type, child);
    source            = move_past(source, maxLength, '[');
    for (int i = 0; i < count; i++)
    {
      source         = read_value(source, maxLength, *structPointer, type, child, size);
      *structPointer = (unsigned char *)(*structPointer + size);
    }
  }
  return source;
}

static unsigned char * read_multiArray(unsigned char * source, unsigned int * maxLength, unsigned char ** structPointer, unsigned int * multiArray, unsigned int index, types_t type, structBody_t * child)
{
  if (index == 1)
  {
    source = read_array(source, maxLength, structPointer, *multiArray, type, child);
  }
  else
  {
    source = move_past(source, maxLength, '[');
    for (size_t i = 0; i < *multiArray; i++)
    {
      source = read_multiArray(source, multiArray, structPointer, multiArray + 1, index - 1, type, child);
    }
    source = move_past(source, maxLength, ']');
  }
}

char * read_struct_from_json(char * source, unsigned int * maxLength, unsigned char * structPointer, structBody_t * structBody)
{
  char * name;
  source = read_start(source, maxLength);
  do
  {
    source = get_string(source, maxLength, &name);
    if (name == NULL)
      break;
    source = move_past_divider(source, maxLength);
    // printf("member name: %s\n", name);
    const structMember_t * member = getMember(structBody, name);
    // printf("member %x\n", member);
    if (member != NULL)
    {
      if (member->type & types_multiArray)
      {
        unsigned char * data = structPointer + member->offset;
        source               = read_multiArray(source, maxLength, &data, member->multi->lengths, member->multi->count, member->type, member->child);
      }
      else if (member->type & types_array)
      {
        unsigned char * data = structPointer + member->offset;
        source               = read_array(source, maxLength, &data, member->count, member->type, member->child);
      }
      else
        source = read_value(source, maxLength, structPointer + member->offset, member->type, member->child, member->count);
    }
    else
    {
      // TODO eat the rest.
    }
  } while (*maxLength > 0);

  return source;
}
