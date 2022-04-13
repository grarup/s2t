#ifndef _JSON_2_STRUCT_H
#define _JSON_2_STRUCT_H

#include "struct2text.h"

char * read_start(char * source, unsigned int * maxLength);
char * read_struct(char * source, unsigned int * maxLength, unsigned char * structPointer, structBody_t * structBody);

#endif