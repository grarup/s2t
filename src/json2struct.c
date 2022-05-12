/*
  <https://github.com/grarup/s2t>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2022 Peter Grarup <grarup@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "json2struct.h"
#include "string.h"
#include "stdlib.h"

/**
 * @brief move a char pointer past a character
 *
 * @param source start pointer
 * @param lengthRemaining length remaining of string
 * @param c characher to move past
 * @return char* pointer to the position after the charactor
 */
static char * move_past(char * source, unsigned int * lengthRemaining, char c)
{
  while (lengthRemaining > 0 && *source != c)
  {
    source++;
    *lengthRemaining--;
  }
  *lengthRemaining--;
  return source + 1;
}

/**
 * @brief moves past a {
 *
 * @param source start pointer
 * @param lengthRemaining length remaining of string
 * @return char* pointer to the position after the charactor
 */
static inline char * read_start(char * source, unsigned int * lengthRemaining)
{
  return move_past(source, lengthRemaining, '{');
}

/**
 * @brief moves past a :
 *
 * @param source start pointer
 * @param lengthRemaining length remaining of string
 * @return char* pointer to the position after the charactor
 */
static inline char * move_past_divider(char * source, unsigned int * lengthRemaining)
{
  return move_past(source, lengthRemaining, ':');
}

/**
 * @brief moves past the end current element
 *
 * looks for a , or }.
 * in case where a { is met before } it will continue until if finds on more }
 *
 * @param source start pointer
 * @param lengthRemaining length remaining of string
 * @return char* pointer to the position after the charactor
 */
static char * move_past_end(char * source, unsigned int * lengthRemaining)
{
  int braced_count = 0;
  while (*lengthRemaining > 1 && (braced_count > 0 || !(*source == ',' || *source == '}')))
  {
    if (*source == '{')
      braced_count++;
    else if (*source == '}')
      braced_count--;
    source++;
    *lengthRemaining--;
  }
  *lengthRemaining--;
  return source + 1;
}

/**
 * @brief findes the first ocourens of a charactor
 *
 * @param source start pointer
 * @param lengthRemaining length remaining of string
 * @param c character to find
 * @return char* pointer to whree the charctor was found else NULL
 */
static char * strnchr(char * source, unsigned int lengthRemaining, char c)
{
  while (lengthRemaining > 0 && *source != c && *source != 0)
  {
    source++;
    lengthRemaining--;
  }
  if (*source != c)
    source = NULL;
  return source;
}

/**
 * @brief returns a pointer to the start of a string and zero terminates it.
 *
 * @param source pointer to string
 * @param remainingLength length left of string
 * @param string pointer to pointer to put the string in.
 * @return char* pointer to the position after the string
 */
static char * get_string(char * source, unsigned int * remainingLength, char ** string)
{
  // TOOD make i handle escaped "
  *string = strnchr(source, *remainingLength, '"');
  if (*string != NULL)
  {
    *string += 1;
    *remainingLength -= ((unsigned int)source - (unsigned int)*string);
    char * end = strnchr(*string, *remainingLength, '"');
    *end       = 0;
    *remainingLength -= (*string - end) + 1;
    return end + 1;
  }
  return source;
}

/**
 * @brief get the pointer to the start of a value string and zero terminates it
 *
 * @param source pointer to string
 * @param remainingLength length left of string
 * @param string pointer to pointer to put the string in.
 * @return char* pointer to the position after the value
 */
static unsigned char * get_value_string(unsigned char * source, unsigned int * remainingLength, char ** string)
{
  char * end = source;
  while (*remainingLength > 0 && !(*end == ',' || *end == '}' || *end == ']'))
  {
    *remainingLength--;
    end++;
  }
  *string = source;
  *end    = 0;
  return end + 1;
}

/**
 * @brief reads a value from json and puts it into the struct
 *
 * @param source pointer to string
 * @param remainingLength length left of string
 * @param memberPointer pointer to
 * @param type type of the member
 * @param child pointer to body if type is a struct
 * @param count count of the member used for char arrays
 * @return char* pointer to the position after the value
 */
static unsigned char * read_value(char * source, unsigned int * lengthRemaining, unsigned char * memberPointer, types_t type, structBody_t * child, int count)
{
  char * szValue;

  if (type & types_pointer)
  {
    if ((unsigned char *)*((unsigned int *)memberPointer) == NULL)
    {
      return move_past_end(source, lengthRemaining);
    }
    memberPointer = (unsigned char *)*((unsigned int *)memberPointer);
  }
  switch (type & types_typeMask)
  {
    case types_u8:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((unsigned char *)memberPointer) = (unsigned char)atoi(szValue);
      }
      break;
    case types_u16:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((unsigned short *)memberPointer) = (unsigned short)atoi(szValue);
      }
      break;
    case types_u32:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((unsigned int *)memberPointer) = (unsigned int)atoi(szValue);
      }
      break;
    case types_i8:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((char *)memberPointer) = (char)atoi(szValue);
      }
      break;
    case types_i16:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((short *)memberPointer) = (short)atoi(szValue);
      }
      break;
    case types_i32:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((int *)memberPointer) = atoi(szValue);
      }
      break;
    case types_f32:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((float *)memberPointer) = atof(szValue);
      }
      break;
    case types_f64:
      source = get_value_string(source, lengthRemaining, &szValue);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((double *)memberPointer) = strtod(szValue, NULL);
      }
      break;
    case types_char:
      source = get_string(source, lengthRemaining, &szValue);
      source = move_past_end(source, lengthRemaining);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
          *((char *)memberPointer) = szValue[0];
      }
      break;
    case types_sz:
      source = get_string(source, lengthRemaining, &szValue);
      source = move_past_end(source, lengthRemaining);
      if (szValue != NULL)
      {
        if (strncmp(szValue, "null", 4) != 0)
        {
          if (count > 0)
          {
            strncpy((char *)memberPointer, szValue, count);
            ((char *)memberPointer)[count - 1] = 0;
          }
          else
            strcpy((char *)memberPointer, szValue);
        }
      }
      break;
    case types_struct:
      source = read_struct_from_json(source, lengthRemaining, memberPointer, child);
      break;
    default:
      break;
  }
  return source;
}

/**
 * @brief reads an array from json and puts it into the struct
 *
 * @param source pointer to string
 * @param remainingLength length left of string
 * @param memberPointer pointer to
 * @param count count of the member used for char arrays
 * @param type type of the member
 * @param child pointer to body if type is a struct
 * @return char* pointer to the position after the array
 */
static unsigned char * read_array(unsigned char * source, unsigned int * lengthRemaining, unsigned char ** memberPointer, unsigned int count, types_t type, structBody_t * child)
{
  if ((type & types_typeMask) == types_char)
  {
    source = read_value(source, lengthRemaining, *memberPointer, types_sz, NULL, count);
  }
  else
  {
    unsigned int size = getSize(type, child);
    source            = move_past(source, lengthRemaining, '[');
    for (int i = 0; i < count; i++)
    {
      source         = read_value(source, lengthRemaining, *memberPointer, type, child, size);
      *memberPointer = (unsigned char *)(*memberPointer + size);
    }
  }
  return source;
}

/**
 * @brief reads an multi array from json and puts it into the struct
 *
 * @param source pointer to string
 * @param remainingLength length left of string
 * @param memberPointer pointer to
 * @param multiArray pointer to array of dimensions
 * @param index index in the multi array
 * @param type type of the member
 * @param child pointer to body if type is a struct
 * @return char* pointer to the position after the multi array
 */
static unsigned char * read_multiArray(unsigned char * source, unsigned int * lengthRemaining, unsigned char ** memberPointer, unsigned int * multiArray, unsigned int index, types_t type, structBody_t * child)
{
  if (index == 1)
  {
    source = read_array(source, lengthRemaining, memberPointer, *multiArray, type, child);
  }
  else
  {
    source = move_past(source, lengthRemaining, '[');
    for (size_t i = 0; i < *multiArray; i++)
    {
      source = read_multiArray(source, multiArray, memberPointer, multiArray + 1, index - 1, type, child);
    }
    source = move_past(source, lengthRemaining, ']');
  }
}

char * read_struct_from_json(char * source, unsigned int * lengthRemaining, unsigned char * memberPointer, structBody_t * structBody)
{
  char * name;
  source = read_start(source, lengthRemaining);
  do
  {
    source = get_string(source, lengthRemaining, &name);
    if (name == NULL)
      break;
    source                        = move_past_divider(source, lengthRemaining);
    const structMember_t * member = getMember(structBody, name);
    if (member != NULL)
    {
      if (member->type & types_multiArray)
      {
        unsigned char * data = memberPointer + member->offset;
        source               = read_multiArray(source, lengthRemaining, &data, member->multi->lengths, member->multi->count, member->type, member->child);
      }
      else if (member->type & types_array)
      {
        unsigned char * data = memberPointer + member->offset;
        source               = read_array(source, lengthRemaining, &data, member->count, member->type, member->child);
      }
      else
        source = read_value(source, lengthRemaining, memberPointer + member->offset, member->type, member->child, member->count);
    }
    else
    {
      // TODO eat the rest.
    }
  } while (*lengthRemaining > 0);

  return source;
}
