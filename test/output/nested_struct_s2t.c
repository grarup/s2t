#include "nested_struct_s2t.h"
#include "stddef.h"

structMember_t simple_nest_members[] = {
	{"u8", types_u8, offsetof(struct simple_nest, u8), 0, NULL},
	{"u16", types_u16, offsetof(struct simple_nest, u16), 0, NULL},
	{"u32", types_u32, offsetof(struct simple_nest, u32), 0, NULL},
	{"i8", types_i8, offsetof(struct simple_nest, i8), 0, NULL},
	{"i16", types_i16, offsetof(struct simple_nest, i16), 0, NULL},
	{"i32", types_i32, offsetof(struct simple_nest, i32), 0, NULL},
};

structBody_t simple_nest_body = {
	"simple_nest",
	6,
	simple_nest_members,
};

structMember_t nest_members[] = {
	{"u8", types_u8, offsetof(struct nest, u8), 0, NULL},
	{"simple", types_struct, offsetof(struct nest, simple), 0, &simple_nest_body},
};

structBody_t nest_body = {
	"nest",
	2,
	nest_members,
};

