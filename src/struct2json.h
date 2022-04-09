
#ifndef _STRUCT_TO_JSON_H
#define _STRUCT_TO_JSON_H

#include "struct2text.h"

char * json_struct(char * target, unsigned int * targetMaxLength, unsigned char * structPointer, structBody_t * structBody);

#endif