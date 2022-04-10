#include "simple_struct_s2t.h"
#include "stddef.h"

structMember_t simple_members[] = {
	{"u8", types_u8, offsetof(struct simple, u8), .count = 0, NULL},
	{"u16", types_u16, offsetof(struct simple, u16), .count = 0, NULL},
	{"u32", types_u32, offsetof(struct simple, u32), .count = 0, NULL},
	{"i8", types_i8, offsetof(struct simple, i8), .count = 0, NULL},
	{"i16", types_i16, offsetof(struct simple, i16), .count = 0, NULL},
	{"i32", types_i32, offsetof(struct simple, i32), .count = 0, NULL},
	{"c", types_char, offsetof(struct simple, c), .count = 0, NULL},
	{"f32", types_f32, offsetof(struct simple, f32), .count = 0, NULL},
	{"f64", types_f64, offsetof(struct simple, f64), .count = 0, NULL},
	{"u8check", types_u8, offsetof(struct simple, u8check), .count = 0, NULL},
};

structBody_t simple_body = {
	"simple",
	10,
	sizeof(struct simple),
	simple_members,
};

