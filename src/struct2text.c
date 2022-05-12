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
