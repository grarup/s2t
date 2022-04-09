import enum
import re
import argparse
import pydantic
import typing
import os


class TypeEntry(pydantic.BaseModel):
    enum: str
    names: typing.List[str]


class Types(pydantic.BaseModel):
    types: typing.List[TypeEntry]

    def get_type(self, type_name: str) -> TypeEntry:
        return next((e for e in self.types if type_name in e.names), None)


class StructMember(pydantic.BaseModel):
    name: typing.Optional[str] = None
    type: typing.Optional[str] = None
    count: typing.Optional[int] = 0
    child: typing.Optional[str] = "NULL"
    is_pointer: typing.Optional[bool] = False
    arrayLengths: typing.Optional[typing.List[str]] = []


class StructBody(pydantic.BaseModel):
    name: str
    typedef_name: str
    count: typing.Optional[int] = 0
    members: typing.Optional[typing.List[StructMember]] = []


def get_struct(name: str, bodies: typing.List[StructBody]) -> StructBody:
    return next((body for body in bodies if body.name == name), None)


def get_struct_name(name: str, bodies: typing.List[StructBody]) -> str:
    structBody = next((body for body in bodies if body.name == name), None)
    if (structBody == None):
        structBody = next((body for body in bodies if body.typedef_name == name), None)
    return structBody.name if structBody != None else name


def get_structs(fileName: str, types: Types) -> typing.List[StructBody]:
    file = open(fileName, "r")
    text = file.read().replace("\n", "")
    structs = re.findall("(?:typedef) ?struct ([^{]*){([^{]*)}([^;]*);", text)
    structBodies: typing.List[StructBody] = []
    for struct in structs:
        structBody = StructBody(name=struct[0], typedef_name=struct[2].strip())
        structBodies.append(structBody)
    for struct in structs:
        structBody = get_struct(struct[0], structBodies)
        entries = re.findall(" *([^;\*]*) (\*)? *([^;]*);", struct[1])
        for entry in entries:
            structMember = StructMember()
            type = types.get_type(entry[0].strip())
            if (type == None):
                structMember.type = "types_struct"
                structMember.child = f"&{get_struct_name(entry[0].strip(), structBodies)}_body"
            else:
                structMember.type = type.enum

            if (entry[1] != ''):
                if (structMember.type == 'types_char'):
                    structMember.type = 'types_sz'
                structMember.type += ' | types_pointer' if entry[1] != '' else ''

            arrayCount = entry[2].count('[')
            if (arrayCount == 0):
                structMember.name = entry[2]
            elif (arrayCount == 1):
                countstr = re.findall("\[(.*)\]", entry[2])
                structMember.count = int(countstr[0])
                structMember.name = re.findall("([^\[]*)", entry[2])[0]
            else:
                countstr = re.findall("\[([^\[]*)\]", entry[2])
                structMember.count = len(countstr)
                structMember.name = re.findall("([^\[]*)", entry[2])[0]
                structMember.arrayLengths.extend(countstr)
                structMember.type += ' | types_multiArray'

            structBody.members.append(structMember)
            structBody.count += 1
    return structBodies


def convert(file_name: str, types: Types, outputFolder: str = None):
    structBodies = get_structs(file_name, types)
    output_file_name = os.path.splitext(os.path.basename(file_name))[0] + "_s2t"
    c_content = f"#include \"{output_file_name}.h\"\n"
    c_content += f"#include \"stddef.h\"\n\n"
    h_content = f"#ifndef _{output_file_name.capitalize()}_H\n"
    h_content += f"#define _{output_file_name.capitalize()}_H\n\n"
    h_content += f"#include \"struct2text.h\"\n"
    h_content += f"#include \"{os.path.basename(file_name)}\"\n\n"
    for structBody in structBodies:
        for structMember in structBody.members:
            if (len(structMember.arrayLengths)):
                c_content += f"unsigned int {structMember.name}_multiArrayLengths[] = {{\n"
                c_content += f"\t{','.join(structMember.arrayLengths)}\n"
                c_content += "};\n\n"
                c_content += f"structMulitArray_t {structMember.name}_multiArray = {{\n"
                c_content += f"\t{len(structMember.arrayLengths)},\n"
                c_content += f"\t{structMember.name}_multiArrayLengths\n"
                c_content += "};\n\n"

        c_content += f"structMember_t {structBody.name}_members[] = {{\n"
        for structMember in structBody.members:
            c_content += f"\t{{\"{structMember.name}\", "
            c_content += f"{structMember.type}, "
            c_content += f"offsetof(struct {structBody.name}, {structMember.name}), "
            if (len(structMember.arrayLengths) > 0):
                c_content += f".multi = &{structMember.name}_multiArray, "
            else:
                c_content += f".count = {structMember.count}, "
            c_content += f"{structMember.child}}},\n"
        c_content += "};\n\n"
        h_content += f"extern structBody_t {structBody.name}_body;\n"
        c_content += f"structBody_t {structBody.name}_body = {{\n"
        c_content += f"\t\"{structBody.name}\",\n"
        c_content += f"\t{structBody.count},\n"
        c_content += f"\tsizeof(struct {structBody.name}),\n"
        c_content += f"\t{structBody.name}_members,\n"
        c_content += "};\n\n"
    h_content += "\n#endif\n"

    hfile = open(os.path.join(outputFolder, output_file_name + ".h"), "w")
    hfile.write(h_content)
    hfile.close()

    cfile = open(os.path.join(outputFolder, output_file_name + ".c"), "w")
    cfile.write(c_content)
    cfile.close()


def main():
    parser = argparse.ArgumentParser(description="Converts png to bin in lvgl format")
    parser.add_argument(
        "-o", help="output file in not defined the output will be the input file .bin"
    )
    parser.add_argument(
        "-t", help="file containting types"
    )
    parser.add_argument("file", help="the file to be converted")
    args = parser.parse_args()

    convert(args.file, Types.parse_file(args.t), args.o)


if __name__ == "__main__":
    main()
