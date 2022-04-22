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
#include "structs/very_simple_struct.h"
#include "output/very_simple_struct_s2t.h"
#include "../src/struct2json.h"
#include "../src/json2struct.h"
#include "json-maker.h"
#include "string.h"

void test_struct(char * name, unsigned char * filledStructPointer, unsigned char * emptyStructPointer, structBody_t * body)
{
  char   text[1000];
  int    length = sizeof(text);
  char * json   = text;

  json = json_struct(json, &length, filledStructPointer, body, NULL);
  json = json_end(json, &length);
  printf("%s\n", name);
  printf("struct to json\n");
  printf("%s\n\n", text);

  json   = text;
  length = sizeof(text);
  json   = read_struct_from_json(json, &length, emptyStructPointer, body);

  json   = text;
  length = sizeof(text);
  json   = json_struct(json, &length, emptyStructPointer, body, NULL);
  json   = json_end(json, &length);

  printf("struct to json to struct to json\n");
  printf("%s\n\n", text);
}

int main()
{

  very_simple_t very_simple = {8, 16, 32};
  very_simple_t very_simple_empty;

  test_struct("very_simple", (unsigned char *)&very_simple, (unsigned char *)&very_simple_empty, &very_simple_body);

  simple_t simple = {8, 16, 32, -8, -16, -32, 'p', 3.14f, 2.27, 8};
  simple_t simple_empty;

  test_struct("simple", (unsigned char *)&simple, (unsigned char *)&simple_empty, &simple_body);

  nest_t nest = {8, {8, 16, 32, -16, -32, 32, 'c', 32.1f, 64.5, 8}};
  nest_t nest_empty;
  test_struct("nest", (unsigned char *)&nest, (unsigned char *)&nest_empty, &nest_body);

  char           string[] = "Peter";
  unsigned short s        = 50;
  int            i        = 100;
  simple_point_t pointer  = {string, NULL, &i, 200};
  simple_point_t pointer_empty;
  unsigned short s_empty = 2;
  int            i_empty = 3;

  memset(&pointer_empty, 0, sizeof(simple_point_t));
  char string_empty[10];
  pointer_empty.szString = string_empty;
  pointer_empty.pu16     = &s_empty;
  pointer_empty.pi32     = &i_empty;
  test_struct("pointer", (unsigned char *)&pointer, (unsigned char *)&pointer_empty, &simple_point_body);

  array_t array = {8, {1, 2, 3}, {1, 2, 3, 4, 50}, {17, 47}, {{7, 2}, {3, 4}, {5, 6}}, "Sigrid"};
  array_t array_empty;
  memset(&array_empty, 0, sizeof(array_t));
  test_struct("array", (unsigned char *)&array, (unsigned char *)&array_empty, &array_body);

  return 0;
}