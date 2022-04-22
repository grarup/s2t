#include "stddef.h"
#include "array_struct_s2t.h"

unsigned int m_u32_multiArrayLengths[] = {
	3,2
};

structMulitArray_t m_u32_multiArray = {
	2,
	m_u32_multiArrayLengths
};

structMember_t array_members[] = {
	{"u8notarray", types_u8, offsetof(struct array, u8notarray), .count = 0, NULL},
	{"u8", types_u8 | types_array, offsetof(struct array, u8), .count = 3, NULL},
	{"u16", types_u16 | types_array, offsetof(struct array, u16), .count = 5, NULL},
	{"u32", types_u32 | types_array, offsetof(struct array, u32), .count = 2, NULL},
	{"m_u32", types_u32 | types_multiArray, offsetof(struct array, m_u32), .multi = &m_u32_multiArray, NULL},
	{"string", types_char | types_array, offsetof(struct array, string), .count = 32, NULL},
};

structBody_t array_body = {
	"array",
	6,
	sizeof(struct array),
	array_members,
};

