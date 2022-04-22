#ifndef _JSON_2_STRUCT_H
#define _JSON_2_STRUCT_H

#include "struct2text.h"

char * read_struct_from_json(char * source, unsigned int * maxLength, unsigned char * structPointer, structBody_t * structBody);

#endif