import enum
import re
import argparse

from numpy import append
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
    file_required: typing.Optional[str] = None
    arrayLengths: typing.Optional[typing.List[str]] = []


class StructBody(pydantic.BaseModel):
    name: str
    typedef_name: str
    file_name: str
    count: typing.Optional[int] = 0
    members: typing.Optional[typing.List[StructMember]] = []
    used: typing.Optional[bool] = False


def get_struct(name: str, bodies: typing.List[StructBody]) -> StructBody:
    return next((body for body in bodies if body.name == name), None)


def get_struct_name(name: str, bodies: typing.List[StructBody]) -> typing.Tuple[str, str]:
    structBody = next((body for body in bodies if body.name == name), None)
    if (structBody == None):
        structBody = next((body for body in bodies if body.typedef_name == name), None)
    else:
        structBody.used = True
    if (structBody != None):
        structBody.used = True
    return (structBody.name, structBody.file_name) if structBody != None else (name, None)


def get_structs(fileName: str, types: Types, folders: typing.List[str], used: bool = False) -> typing.List[StructBody]:
    file = open(fileName, "r")
    text = file.read().replace("\n", "")
    hfiles = re.findall("#include *\"([^\"]*)\"", text)
    structBodies: typing.List[StructBody] = []
    for hfile in hfiles:
        structBodies.extend(get_structs(find_file(hfile, folders), types, folders))
        print(f"h file: {find_file(hfile, folders)}")

    structs = re.findall("(?:typedef) ?struct ?([^{]*){([^{]*)}([^;]*);", text)
    for struct in structs:
        structBody = StructBody(name=struct[0], typedef_name=struct[2].strip(), file_name=fileName, used=used)
        structBodies.append(structBody)
    for struct in structs:
        structBody = get_struct(struct[0], structBodies)
        entries = re.findall(" *([^;\*]*) (\*)? *([^;]*);", struct[1])
        for entry in entries:
            structMember = StructMember()
            type = types.get_type(entry[0].strip())
            if (type == None):
                structMember.type = "types_struct"
                (name, filename) = get_struct_name(entry[0].strip(), structBodies)
                structMember.child = f"&{name}_body"
                structMember.file_required = os.path.splitext(os.path.basename(filename))[0] + "_s2t.h"
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
                structMember.type += ' | types_array'
            else:
                countstr = re.findall("\[([^\[]*)\]", entry[2])
                structMember.count = len(countstr)
                structMember.name = re.findall("([^\[]*)", entry[2])[0]
                structMember.arrayLengths.extend(countstr)
                structMember.type += ' | types_multiArray'

            structBody.members.append(structMember)
            structBody.count += 1
    return structBodies


def find_file(fileName: str, folders: typing.List[str]) -> str:
    for folder in folders:
        if (os.path.exists(os.path.join(folder, fileName))):
            return os.path.join(folder, fileName)


def convert(file_name: str, types: Types, folders: typing.List[str], recursive: bool = False, outputFolder: str = None):
    structBodies = get_structs(find_file(file_name, folders), types, folders, True)
    c_contents: typing.Dict[str, str] = {}
    c_includes: typing.Dict[str, typing.List[str]] = {}
    h_contents: typing.Dict[str, str] = {}
    for structBody in structBodies:
        if (recursive or file_name == os.path.basename(structBody.file_name)):
            output_file_name = os.path.splitext(os.path.basename(structBody.file_name))[0] + "_s2t"
            if (not output_file_name in c_contents.keys()):
                c_contents[output_file_name] = ""
                c_includes[output_file_name] = []
                c_includes[output_file_name].append(f"stddef.h")
                c_includes[output_file_name].append(f"{output_file_name}.h")
                h_contents[output_file_name] = f"#ifndef _{output_file_name.capitalize()}_H\n"
                h_contents[output_file_name] += f"#define _{output_file_name.capitalize()}_H\n\n"
                h_contents[output_file_name] += f"#include \"struct2text.h\"\n"
                h_contents[output_file_name] += f"#include \"{os.path.basename(file_name)}\"\n\n"

            for structMember in structBody.members:
                if (len(structMember.arrayLengths)):
                    c_contents[output_file_name] += f"unsigned int {structMember.name}_multiArrayLengths[] = {{\n"
                    c_contents[output_file_name] += f"\t{','.join(structMember.arrayLengths)}\n"
                    c_contents[output_file_name] += "};\n\n"
                    c_contents[output_file_name] += f"structMulitArray_t {structMember.name}_multiArray = {{\n"
                    c_contents[output_file_name] += f"\t{len(structMember.arrayLengths)},\n"
                    c_contents[output_file_name] += f"\t{structMember.name}_multiArrayLengths\n"
                    c_contents[output_file_name] += "};\n\n"

            c_contents[output_file_name] += f"structMember_t {structBody.name if (structBody.name != '') else structBody.typedef_name}_members[] = {{\n"
            for structMember in structBody.members:
                c_contents[output_file_name] += f"\t{{\"{structMember.name}\", "
                c_contents[output_file_name] += f"{structMember.type}, "
                if (structBody.name != ""):
                    c_contents[output_file_name] += f"offsetof(struct {structBody.name}, {structMember.name}), "
                else:
                    c_contents[output_file_name] += f"offsetof({structBody.typedef_name}, {structMember.name}), "

                if (len(structMember.arrayLengths) > 0):
                    c_contents[output_file_name] += f".multi = &{structMember.name}_multiArray, "
                else:
                    c_contents[output_file_name] += f".count = {structMember.count}, "
                c_contents[output_file_name] += f"{structMember.child}}},\n"
                if (structMember.file_required != None and not (structMember.file_required in c_includes[output_file_name])):
                    c_includes[output_file_name].append(structMember.file_required)
            c_contents[output_file_name] += "};\n\n"
            h_contents[output_file_name] += f"extern structBody_t {structBody.name if (structBody.name != '') else structBody.typedef_name}_body;\n"
            c_contents[output_file_name] += f"structBody_t {structBody.name if (structBody.name != '') else structBody.typedef_name}_body = {{\n"
            c_contents[output_file_name] += f"\t\"{structBody.name if (structBody.name != '') else structBody.typedef_name}\",\n"
            c_contents[output_file_name] += f"\t{structBody.count},\n"
            if (structBody.name != ""):
                c_contents[output_file_name] += f"\tsizeof(struct {structBody.name}),\n"
            else:
                c_contents[output_file_name] += f"\tsizeof({structBody.typedef_name}),\n"
            c_contents[output_file_name] += f"\t{structBody.name if (structBody.name != '') else structBody.typedef_name}_members,\n"
            c_contents[output_file_name] += "};\n\n"

    for key in h_contents.keys():
        print(f"File Created: {key}.h")
        h_contents[key] += "\n#endif\n"
        hfile = open(os.path.join(outputFolder, key + ".h"), "w")
        hfile.write(h_contents[key])
        hfile.close()
        print(f"File Created: {key}.c")
        cfile = open(os.path.join(outputFolder, key + ".c"), "w")
        for include in c_includes[key]:
            cfile.write(f"#include \"{include}\"\n")
        cfile.write("\n")
        cfile.write(c_contents[key])
        cfile.close()


def main():
    parser = argparse.ArgumentParser(description="Generates body for struct ")
    parser.add_argument(
        "-o", "--outputfolder", help="output folder, if not set the files will be placed nexted."
    )
    parser.add_argument(
        "-t", "--types", help="file containting types", required=True
    )
    parser.add_argument("-r", "--recursive", help="look for structs recursiv", action="store_true")
    parser.add_argument("-s", "--structs", help="structs to generate for. If none is set all will be extracted", action="append")
    parser.add_argument("-f", "--folders", help="folders to look for h files", action="append")
    parser.add_argument("file", help="the file containing the struct")
    args = parser.parse_args()

    convert(args.file, Types.parse_file(args.t), args.f, args.r, args.o)


if __name__ == "__main__":
    main()
