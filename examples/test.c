#include <stdio.h>
#include "struct2json.h"
#include "json2struct.h"
#include "json-maker.h"
#include "string.h"
#include "example1.h"
#include "example1_s2t.h"

void test_struct_2_json()
{
  example1_t example1 = {8, "Hello world!"};
  char       text[100];
  char *     json   = text;
  int        length = sizeof(text);

  json = json_struct(json, &length, (unsigned char *)&example1, &example1_body, NULL);
  json = json_end(json, &length);

  printf("%s\n", text);
}

void test_json_2_struct()
{
  example1_t example1;
  char       text[100] = "{\"u8\":8,\"string\":\"Hello world!\"}";
  char *     json      = text;
  int        length    = strlen(json);
  json                 = read_struct_from_json(json, &length, (unsigned char *)&example1, &example1_body);

  printf("u8: %i\n", example1.u8);
  printf("string: %s\n", example1.string);
}

int main()
{
  test_struct_2_json();
  test_json_2_struct();
  return 0;
}