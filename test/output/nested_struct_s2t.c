#include "stddef.h"
#include "nested_struct_s2t.h"
#include "simple_struct_s2t.h"

structMember_t nest_members[] = {
	{"u8", types_u8, offsetof(struct nest, u8), .count = 0, NULL},
	{"simple", types_struct, offsetof(struct nest, simple), .count = 0, &simple_body},
};

structBody_t nest_body = {
	"nest",
	2,
	sizeof(struct nest),
	nest_members,
};

