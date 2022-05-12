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
#include "structs/string_struct.h"
#include "output/string_struct_s2t.h"
#include "../src/struct2json.h"
#include "../src/json2struct.h"
#include "json-maker.h"
#include "string.h"

int test_json2Struct(char * name, char * json, unsigned char * structPointer, structBody_t * body, char * expectedResult)
{
  char text[1000];
  int  length = sizeof(text);

  length = strlen(json);
  json   = read_struct_from_json(json, &length, structPointer, body);

  json   = text;
  length = sizeof(text);
  json   = json_struct(json, &length, structPointer, body, NULL);
  json   = json_end(json, &length);

  printf("struct to json to struct to json\n");
  printf("Expected: %s\n", expectedResult);
  printf("Actually: %s\n", text);
  printf("Result: %s\n\n", strncmp(text, expectedResult, sizeof(text)) == 0 ? "Pass" : "Failed");
  return (strncmp(text, expectedResult, sizeof(text)) == 0);
}

int test_struct2json(char * name, unsigned char * structPointer, char * json, int jsonMaxLength, structBody_t * body, char * expectedResult)
{
  int    length    = jsonMaxLength;
  char * jsonStart = json;
  json             = json_struct(json, &length, structPointer, body, NULL);
  json             = json_end(json, &length);
  printf("%s\n", name);
  printf("struct to json\n");
  printf("Expected: %s\n", expectedResult);
  printf("Actually: %s\n", jsonStart);
  int result = strncmp(jsonStart, expectedResult, jsonMaxLength) == 0;
  printf("Result: %s\n\n", result ? "Pass" : "Failed");
  return result;
}

int test_struct(char * name, unsigned char * filledStructPointer, unsigned char * emptyStructPointer, structBody_t * body, char * expectedResult1, char * expectedResult2)
{
  char json[1000];
  test_struct2json(name, filledStructPointer, json, sizeof(json), body, expectedResult1);
  test_json2Struct(name, json, emptyStructPointer, body, expectedResult2);
}

int main()
{
  int result = 1;
  // Very simple
  very_simple_t very_simple = {8, 16, 32};
  very_simple_t very_simple_empty;
  char *        very_simple_result = "{\"u8\":8,\"u16\":16,\"u32\":32}";

  result &= test_struct("very_simple", (unsigned char *)&very_simple, (unsigned char *)&very_simple_empty, &very_simple_body, very_simple_result, very_simple_result);

  // Simple
  simple_t simple = {8, 16, 32, -8, -16, -32, 'p', 3.14f, 2.27, 8};
  simple_t simple_empty;
  char *   simple_result = "{\"u8\":8,\"u16\":16,\"u32\":32,\"i8\":-8,\"i16\":-16,\"i32\":-32,\"c\":\"p\",\"f32\":3.14,\"f64\":2.27,\"u8check\":8}";

  result &= test_struct("simple", (unsigned char *)&simple, (unsigned char *)&simple_empty, &simple_body, simple_result, simple_result);

  // Nested
  nest_t nest = {8, {8, 16, 32, -16, -32, 32, 'c', 32.1f, 64.5, 8}};
  nest_t nest_empty;
  char * nest_result = "{\"u8\":8,\"simple\":{\"u8\":8,\"u16\":16,\"u32\":32,\"i8\":-16,\"i16\":-32,\"i32\":32,\"c\":\"c\",\"f32\":32.1,\"f64\":64.5,\"u8check\":8}}";

  result &= test_struct("nest", (unsigned char *)&nest, (unsigned char *)&nest_empty, &nest_body, nest_result, nest_result);

  // Pointer
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
  char * pointer_result1 = "{\"szString\":\"Peter\",\"pu16\":null,\"pi32\":100,\"i32\":200}";
  char * pointer_result2 = "{\"szString\":\"Peter\",\"pu16\":2,\"pi32\":100,\"i32\":200}";

  result &= test_struct("pointer", (unsigned char *)&pointer, (unsigned char *)&pointer_empty, &simple_point_body, pointer_result1, pointer_result2);

  // Array
  array_t array = {8, {1, 2, 3}, {1, 2, 3, 4, 50}, {17, 47}, {{7, 2}, {3, 4}, {5, 6}}, "Sigrid"};
  array_t array_empty;
  memset(&array_empty, 0, sizeof(array_t));

  char * array_result = "{\"u8notarray\":8,\"u8\":[1,2,3],\"u16\":[1,2,3,4,50],\"u32\":[17,47],\"m_u32\":[[7,2],[3,4],[5,6]],\"string\":\"Sigrid\"}";

  result &= test_struct("array", (unsigned char *)&array, (unsigned char *)&array_empty, &array_body, array_result, array_result);

  // String
  string_t string_struct = {string, "1234"};
  char     emptyString[30];
  memset(emptyString, 0, sizeof(emptyString));
  string_t empty_string_struct;
  empty_string_struct.pointerToString = emptyString;

  char * string_result = "{\"pointerToString\":\"Peter\",\"string\":\"1234\"}";

  result &= test_struct("string", (unsigned char *)&string_struct, (unsigned char *)&empty_string_struct, &string_body, string_result, string_result);

  // To long string
  memset(&empty_string_struct, 0, sizeof(empty_string_struct));
  memset(emptyString, 0, sizeof(emptyString));
  empty_string_struct.pointerToString = emptyString;

  char json[1000] = "{\"pointerToString\":\"Peter\",\"string\":\"123456789\"}";

  result &= test_json2Struct("long string", json, (unsigned char *)&empty_string_struct, &string_body, string_result);

  // Extra json
  memset(&empty_string_struct, 0, sizeof(empty_string_struct));
  memset(emptyString, 0, sizeof(emptyString));
  empty_string_struct.pointerToString = emptyString;

  char json_extra[1000] = "{\"pointerToString\":\"Peter\",\"extra_int\":1,\"extra_struct\":{\"pointerToString\":\"Hmm\"},\"extra_array\":[1,2,3],\"string\":\"1234\"}";
  printf("%s\n", json_extra);

  result &= test_json2Struct("extra json", json_extra, (unsigned char *)&empty_string_struct, &string_body, string_result);

  // Final result
  printf("Final result: %s\n", result ? "Passed" : "Failed");

  return result ? 0 : 1;
}