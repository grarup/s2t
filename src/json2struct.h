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

#ifndef _JSON_2_STRUCT_H
#define _JSON_2_STRUCT_H

#include "struct2text.h"

/**
 * @brief read json an puts it into a struct.
 *
 * The json will be destroyed as zero termaination i done in the string to avoid
 * copying the data.
 *
 * @param source  json to convert
 * @param sourceRemaningLength remaining length of source
 * @param structPointer pointer to the struct to convert
 * @param structBody pointer to the struct body to use for the conversion
 * @return char* pointer to after the text used in this source.
 */
char * read_struct_from_json(char * source, unsigned int * sourceRemaningLength, unsigned char * structPointer, structBody_t * structBody);

#endif