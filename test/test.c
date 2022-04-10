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
#include "json-maker.h"

int main()
{
  int    length = 1000;
  char   text[1000];
  char * json = text;

  simple_t simple = {8, 16, 32, -8, -16, -32, 'p', 3.14f, 2.27, 8};

  json = json_objOpen(json, NULL, &length);
  json = json_struct(json, &length, (unsigned char *)&simple, &simple_body);
  json = json_end(json, &length);

  printf("%s\n\n", text);

  nest_t nest = {8, {8, 16, 32, -16, -32}};
  length      = 1000;
  json        = text;

  json = json_objOpen(json, NULL, &length);
  json = json_struct(json, &length, (unsigned char *)&nest, &nest_body);
  json = json_end(json, &length);

  printf("%s\n\n", text);

  char           string[] = "Peter";
  unsigned short s        = 50;
  int            i        = 100;
  simple_point_t pointer  = {string, NULL, &i, 200};
  length                  = 1000;
  json                    = text;

  json = json_objOpen(json, NULL, &length);
  json = json_struct(json, &length, (unsigned char *)&pointer, &simple_point_body);
  json = json_end(json, &length);

  printf("%s\n\n", text);

  array_t array = {8, {1, 2, 3}, {1, 2, 3, 4, 50}, {17, 47}, {{7, 2}, {3, 4}, {5, 6}}, {'S', 'i', 'g', 'r', 'i', 'd', 0}};
  length        = 1000;
  json          = text;

  json = json_objOpen(json, NULL, &length);
  json = json_struct(json, &length, (unsigned char *)&array, &array_body);
  json = json_end(json, &length);
  printf("%s\n", text);

  return 0;
}