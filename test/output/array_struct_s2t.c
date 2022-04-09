#include "array_struct_s2t.h"
#include "stddef.h"

unsigned int m_u32_multiArrayLengths[] = {
	3,2
};

structMember_t array_members[] = {
	{"u8notarray", types_u8, offsetof(struct array, u8notarray), 0, NULL},
	{"u8", types_u8, offsetof(struct array, u8), 3, NULL},
	{"u16", types_u16, offsetof(struct array, u16), 5, NULL},
	{"u32", types_u32, offsetof(struct array, u32), 2, NULL},
	{"m_u32", types_u32 | types_multiArray, offsetof(struct array, m_u32), 2, m_u32_multiArrayLengths},
};

structBody_t array_body = {
	"array",
	5,
	array_members,
};

