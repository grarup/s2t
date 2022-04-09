#include "pointer_struct_s2t.h"
#include "stddef.h"

structMember_t simple_point_members[] = {
	{"szString", types_sz | types_pointer, offsetof(struct simple_point, szString), .count = 0, NULL},
	{"pu16", types_u16 | types_pointer, offsetof(struct simple_point, pu16), .count = 0, NULL},
	{"pi32", types_i32 | types_pointer, offsetof(struct simple_point, pi32), .count = 0, NULL},
	{"i32", types_i32, offsetof(struct simple_point, i32), .count = 0, NULL},
};

structBody_t simple_point_body = {
	"simple_point",
	4,
	sizeof(struct simple_point),
	simple_point_members,
};

