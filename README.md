# s2t

s2t (pronouced stot) is made to make it easy to convert from a c struct to a text based data format and back.

For now the conversion is only implemented for Json.

## Example

### Setup

We define a struct

```c
typedef struct example1
{
  unsigned char u8;
  char         string[32];
} example1_t;
```

For this struct we need to define a description of the struct. This can be done automatically with the python script.

```c
structMember_t example1_members[] = {
    {"u8", types_u8, offsetof(struct example1, u8), .count = 0, NULL},
    {"string", types_char | types_array, offsetof(struct example1, string), .count = 32, NULL},
};

structBody_t example1_body = {
    "example1",
    2,
    sizeof(struct example1),
    example1_members,
};
```

### Struct to json

When this is ready we can convert a struct to json like this.

```c
example1_t example1 = {8, "Hello world!"};
char       text[100];
char *     json   = text;
int        length = sizeof(text);

json = json_struct(json, &length, (unsigned char *)&example1, &example1_body, NULL);
json = json_end(json, &length);

printf("%s\n", text);
```

output:

```json
{"u8":8,"string":"Hello world!"}
```

### Json to struct

Loading json into the struct is done like this.

```c
example1_t example1;
char       text[100] = "{\"u8\":8,\"string\":\"Hello world!\"}";
char *     json      = text;
int        length    = strlen(json);
json                 = read_struct(json, &length, (unsigned char *)&example1, &example1_body);

printf("u8: %i\n", example1.u8);
printf("string: %s\n", example1.string);
```

output:

```cmd
u8: 8
string: Hello world!
```

### Python tool to generate the bodies

To easy create the struct needed for the conversion a python script can be run.

```
python.exe ..\tools\struct2text.py example1.h -t ..\tools\types.json -o . -f . 
```

Help for the script:

```
python.exe ..\tools\struct2text.py -h
usage: struct2text.py [-h] [-o OUTPUTFOLDER] -t TYPES [-r] [-s STRUCTS]
                      [-f FOLDERS]
                      file

Generates body for struct

positional arguments:
  file                  the file containing the struct

optional arguments:
  -h, --help            show this help message and exit
  -o OUTPUTFOLDER, --outputfolder OUTPUTFOLDER
                        output folder, if not set the files will be placed
                        nexted.
  -t TYPES, --types TYPES
                        file containting types
  -r, --recursive       look for structs recursiv
  -s STRUCTS, --structs STRUCTS
                        structs to generate for. If none is set all will be
                        extracted
  -f FOLDERS, --folders FOLDERS
                        folders to look for h files
```
