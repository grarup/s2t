#define NOMINMAX
#include <stdio.h>
#include "structs/simple_struct.h"
#include "output/simple_struct_s2t.h"
#include "structs/nested_struct.h"
#include "output/nested_struct_s2t.h"
#include "structs/pointer_struct.h"
#include "output/pointer_struct_s2t.h"
#include "structs/array_struct.h"
#include "output/array_struct_s2t.h"
#include "../src/struct2json.h"

int main()
{
  int  length = 1000;
  char json[1000];

  simple_t simple = {8, 16, 32, -8, -16, -32};
  json_struct(json, &length, (unsigned char *)&simple, &simple_body);
  printf("%s\n", json);

  nest_t nest = {8, {8, 16, 32, -8, -16, -32}};
  length      = 1000;
  json_struct(json, &length, (unsigned char *)&nest, &nest_body);
  printf("%s\n", json);

  char           string[] = "Peter";
  unsigned short s        = 50;
  int            i        = 100;
  simple_point_t pointer  = {string, NULL, &i, 200};
  length                  = 1000;
  json_struct(json, &length, (unsigned char *)&pointer, &simple_point_body);
  printf("%s\n", json);

  array_t array = {8, {1, 2, 3}, {1, 2, 3, 4, 50}, {17, 47}, {{7, 2}, {3, 4}, {5, 6}}};
  length        = 1000;
  json_struct(json, &length, (unsigned char *)&array, &array_body);
  printf("%s\n", json);

  return 0;
}