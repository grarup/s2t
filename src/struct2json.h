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

#ifndef _STRUCT_TO_JSON_H
#define _STRUCT_TO_JSON_H

#include "struct2text.h"

/**
 * @brief converts a struct to json
 *
 * @param distination a pointer to the char array where the json should be placed
 * @param destinationRemainingLength pointer to a variable containing the max number of
 *                                   characters left in the distination buffer
 * @param structPointer pointer to struct to convert
 * @param structBody pointer to the body containing information about the struct
 * @param name name of the struct, if NULL will be created with a name.
 * @return char* pointer to the null character of the destination string.
 */
char * json_struct(char * distination, unsigned int * destinationRemainingLength, unsigned char * structPointer, structBody_t * structBody, char * name);

#endif