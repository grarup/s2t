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

#include "struct2json.h"
#include "string.h"
#include "json-maker.h"

/**
 * @brief generates json for a struct member
 *
 * @param distination a pointer to the char array where the json should be placed
 * @param destinationRemainingLength pointer to a variable containing the max number of
 *                                   characters left in the distination buffer
 * @param name name of the member
 * @param data pointer to data for member
 * @param type type of member
 * @param child child body if type is struct.
 * @return char* pointer to the null character of the destination string.
 */
char * json_member(char * destination, unsigned int * destinationRemainingLength, char * name, unsigned char * data, types_t type, void * child)
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
      destination = json_uint(destination, name, *valuePointer, destinationRemainingLength);
      break;
    case types_u16:
      destination = json_uint(destination, name, *((unsigned short *)valuePointer), destinationRemainingLength);
      break;
    case types_u32:
      destination = json_uint(destination, name, *((unsigned int *)valuePointer), destinationRemainingLength);
      break;
    case types_i8:
      destination = json_int(destination, name, *((char *)valuePointer), destinationRemainingLength);
      break;
    case types_i16:
      destination = json_int(destination, name, *((short *)valuePointer), destinationRemainingLength);
      break;
    case types_i32:
      destination = json_int(destination, name, *((int *)valuePointer), destinationRemainingLength);
      break;
    case types_f32:
      destination = json_double(destination, name, *((float *)valuePointer), destinationRemainingLength);
      break;
    case types_f64:
      destination = json_double(destination, name, *((double *)valuePointer), destinationRemainingLength);
      break;
    case types_char:
      destination = json_nstr(destination, name, valuePointer, 1, destinationRemainingLength);
      break;
    case types_sz:
      destination = json_str(destination, name, valuePointer, destinationRemainingLength);
      break;
    case types_struct:
      destination = json_struct(destination, destinationRemainingLength, valuePointer, child, name);
      break;
    default:
      destination = json_null(destination, name, destinationRemainingLength);
      break;
  }
  return destination;
}

/**
 * @brief generates json form an array
 *
 * @param distination a pointer to the char array where the json should be placed
 * @param destinationRemainingLength pointer to a variable containing the max number of
 *                                   characters left in the distination buffer
 * @param name name of the member
 * @param data pointer to data for member
 * @param count number of elements in struct
 * @param type type of member
 * @param child child body if type is struct.
 * @return char* pointer to the null character of the destination string.
 */
static char * json_array(char * destination, unsigned int * destinationRemainingLength, char * name, unsigned char ** data, int count, types_t type, void * child)
{
  if ((type & types_typeMask) == types_char)
  {
    destination = json_nstr(destination, name, *data, count, destinationRemainingLength);
    *data       = (unsigned char *)(*data + count);
    return destination;
  }
  destination = json_arrOpen(destination, name, destinationRemainingLength);
  for (int i = 0; i < count; i++)
  {
    destination       = json_member(destination, destinationRemainingLength, NULL, *data, type, child);
    unsigned int size = getSize(type, child);
    *data             = (unsigned char *)(*data + size);
  }
  destination = json_arrClose(destination, destinationRemainingLength);
  return destination;
}

/**
 * @brief generates json for a mulit array
 *
 * @param distination a pointer to the char array where the json should be placed
 * @param destinationRemainingLength pointer to a variable containing the max number of
 *                                   characters left in the distination buffer
 * @param name name of the member
 * @param arrayPointer pointer to a pointer of where the data for the array is stored
 *                     this is increased by the amount that is read
 * @param count number of dimentions of the mulit array
 * @param type type of member
 * @param child child body if type is struct.
 * @param arrayDim array of length for the array
 * @return char* pointer to the null character of the destination string.
 */
char * json_multi_array(char * destination, unsigned int * destinationRemainingLength, char * name, unsigned char ** arrayPointer, int count, types_t type, void * child, unsigned int * arrayDim)
{
  if (count == 1)
  {
    destination = json_array(destination, destinationRemainingLength, NULL, arrayPointer, *arrayDim, type, child);
  }
  else
  {
    destination = json_arrOpen(destination, name, destinationRemainingLength);
    for (size_t i = 0; i < *arrayDim; i++)
    {
      destination = json_multi_array(destination, destinationRemainingLength, NULL, arrayPointer, count - 1, type, child, arrayDim + 1);
    }

    destination = json_arrClose(destination, destinationRemainingLength);
  }
  return destination;
}

char * json_struct(char * destination, unsigned int * destinationRemainingLength, unsigned char * structPointer, structBody_t * structBody, char * name)
{
  unsigned char * basePointer = structPointer;
  destination                 = json_objOpen(destination, name, destinationRemainingLength);
  unsigned char * data        = structPointer;
  for (unsigned int i = 0; i < structBody->count; i++)
  {
    structMember_t * member = (structMember_t *)&structBody->members[i];
    unsigned char *  data   = (unsigned char *)(basePointer + member->offset);
    if (member->type & types_multiArray)
    {
      destination = json_multi_array(destination, destinationRemainingLength, member->name, &data, member->multi->count, member->type & types_typeMask, NULL, member->multi->lengths);
    }
    else if (member->type & types_array)
    {
      destination = json_array(destination, destinationRemainingLength, member->name, &data, member->count, member->type, member->child);
    }
    else
    {
      destination = json_member(destination, destinationRemainingLength, member->name, data, member->type, member->child);
    }
  }
  destination = json_objClose(destination, destinationRemainingLength);
}