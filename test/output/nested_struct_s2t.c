#include "nested_struct_s2t.h"
#include "stddef.h"

structMember_t simple_nest_members[] = {
	{"u8", types_u8, offsetof(struct simple_nest, u8), .count = 0, NULL},
	{"u16", types_u16, offsetof(struct simple_nest, u16), .count = 0, NULL},
	{"u32", types_u32, offsetof(struct simple_nest, u32), .count = 0, NULL},
	{"i16", types_i16, offsetof(struct simple_nest, i16), .count = 0, NULL},
	{"i32", types_i32, offsetof(struct simple_nest, i32), .count = 0, NULL},
};

structBody_t simple_nest_body = {
	"simple_nest",
	5,
	sizeof(struct simple_nest),
	simple_nest_members,
};

structMember_t nest_members[] = {
	{"u8", types_u8, offsetof(struct nest, u8), .count = 0, NULL},
	{"simple", types_struct, offsetof(struct nest, simple), .count = 0, &simple_nest_body},
};

structBody_t nest_body = {
	"nest",
	2,
	sizeof(struct nest),
	nest_members,
};

