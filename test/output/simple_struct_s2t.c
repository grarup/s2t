#include "simple_struct_s2t.h"
#include "stddef.h"

structMember_t simple_members[] = {
	{"u8", types_u8, offsetof(struct simple, u8), 0, NULL},
	{"u16", types_u16, offsetof(struct simple, u16), 0, NULL},
	{"u32", types_u32, offsetof(struct simple, u32), 0, NULL},
	{"i8", types_i8, offsetof(struct simple, i8), 0, NULL},
	{"i16", types_i16, offsetof(struct simple, i16), 0, NULL},
	{"i32", types_i32, offsetof(struct simple, i32), 0, NULL},
};

structBody_t simple_body = {
	"simple",
	6,
	simple_members,
};

