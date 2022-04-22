#include "stddef.h"
#include "example1_s2t.h"

structMember_t example1_members[] = {
	{"u8", types_u8, offsetof(struct example1, u8), .count = 0, NULL},
	{"string", types_char | types_array, offsetof(struct example1, string), .count = 32, NULL},
};

structBody_t example1_body = {
	"example1",
	2,
	sizeof(struct example1),
	example1_members,
};

