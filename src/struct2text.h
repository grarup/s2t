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

#ifndef _STRUCT_TO_TEXT_H
#define _STRUCT_TO_TEXT_H

/**
 * @brief Enum to define the type of members, flags are used for array and pointers.
 *
 */
typedef enum types
{
  types_null,
  types_u8,
  types_u16,
  types_u32,
  types_i8,
  types_i16,
  types_i32,
  types_f32,
  types_f64,
  types_sz,
  types_char,
  types_struct,
  types_typeMask   = 0x1FFFFFFF,
  types_array      = 0x20000000,
  types_multiArray = 0x40000000,
  types_pointer    = 0x80000000
} types_t;

/**
 * @brief Holds information for a multi array
 *
 * count is the depth of the multi array.
 * lengths are an array of the count in each step of the mulit array
 *
 * int test[10][20][2]
 *
 * count = 3
 * lengths = [10,20,2]
 */
typedef struct structMulitArray
{
  unsigned int   count;   // depth of array
  unsigned int * lengths; // lengths of dimensions of the array
} structMulitArray_t;

// Forward declaration
struct structBody;

/**
 * @brief Holds information about a single member in the struct.
 *
 */
typedef struct structMember
{
  char *       name;
  types_t      type;
  unsigned int offset; // offset from the root of the struct in bytes
  union
  {
    unsigned int         count; // count for array
    structMulitArray_t * multi; // information for mulitarrays
  };
  struct structBody * child; // child struct if not a simple type
} structMember_t;

/**
 * @brief Holds information about the struct it self.
 *
 */
typedef struct structBody
{
  unsigned int           count;   // count of members
  unsigned int           size;    // size of the struct in bytes
  const structMember_t * members; // array of members
} structBody_t;

/**
 * @brief Get the Size of a member
 *
 * @param type the type of the member
 * @param child the child of the member if it is a struct.
 * @return the size of the member in bytes.
 */
unsigned int getSize(types_t type, structBody_t * child);

/**
 * @brief Get the Member from name
 *
 * @param body body of the struct
 * @param name name of the meber to finde
 * @return const structMember_t* a pointer to the member.
 */
const structMember_t * getMember(structBody_t * body, char * name);

#endif