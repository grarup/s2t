#include "stddef.h"
#include "very_simple_struct_s2t.h"

structMember_t very_simple_members[] = {
	{"u8", types_u8, offsetof(struct very_simple, u8), .count = 0, NULL},
	{"u16", types_u16, offsetof(struct very_simple, u16), .count = 0, NULL},
	{"u32", types_u32, offsetof(struct very_simple, u32), .count = 0, NULL},
};

structBody_t very_simple_body = {
	"very_simple",
	3,
	sizeof(struct very_simple),
	very_simple_members,
};

