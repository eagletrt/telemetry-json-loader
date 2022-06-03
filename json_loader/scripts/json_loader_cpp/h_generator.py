from ..utils import gen_types
from ..utils import templates
from ..utils import type_model
from ..utils.utils import *

_h_manager = gen_types.TypeManager()

def gen_header(is_loader=False, include_json=True):
    ret = ""
    if not is_loader:
        ret += "#pragma once\n\n"

    ret += "#include <stdio.h>\n"
    ret += "#include <stdlib.h>\n"
    ret += "#include <string>\n"
    ret += "#include <vector>\n"
    ret += "#include <fstream>\n"
    ret += "#include <iostream>\n"
    ret += "#include <exception>\n"
    ret += "#include <iomanip>\n"
    ret += "\n"
    if include_json:
        ret += "#include \"nlohmann_json.hpp\"\n"
        ret += "using json = nlohmann::json;\n"

    if is_loader:
        ret += "#include \"json_models.h\"\n"

    ret += "\n\n"
    return ret

def gen_common_func():
    ret = ""
    ret += "template<class T>\n"
    ret += "void CheckJson(const T& obj, const json& j);\n"
    ret += "template<class T>\n"
    ret += "json Serialize(const T& obj);\n"
    ret += "template<class T>\n"
    ret += "void Deserialize(T& obj, const json& j);\n"
    ret += "template<class T>\n"
    ret += "bool LoadJson(T& obj, const std::string& path);\n"
    ret += "template<class T>\n"
    ret += "void SaveJson(const T& obj, const std::string& path);\n"
    ret += "\n"
    return ret

def gen_struct(class_: type_model.Type) -> str:
    ret = ""
    ret += "struct {}\n".format(class_.name if class_.typename == "" else class_.typename)
    ret += "{\n"
    for field in class_.content:
        if isinstance(field, type_model.VectorType):
            _h_manager.Set("vector")
            cpp_vec = _h_manager.CppType()

            last_type, count = field.get_vector_recursion()

            typename = ""
            if isinstance(last_type, type_model.ObjectType):
                typename = last_type.typename
            else:
                typename = last_type.typename
            ret += "\t"+(cpp_vec + "<")*count + typename + ">"*count + f" {field.name};\n"
        elif isinstance(field, type_model.ObjectType):
            ret += "\t{} {};\n".format(field.typename, field.name)
        else:
            ret += "\t{} {};\n".format(field.typename, field.name)
    ret += "};\n\n"
    return ret


def h_generator(path, classes):

    check_or_create_dir(path)

    content = ""
    content += gen_header(False, False)
    for class_ in classes:
        content += gen_struct(class_)
    fout = open(path + "/json_models.h", "w")
    fout.write(content)
    fout.close()


    content = ""
    content += gen_header(True)
    content += "\n\n"
    content += "#ifndef JSON_LOADER_H\n"
    content += "#define JSON_LOADER_H\n\n"
    content += gen_common_func()
    content += "#endif // JSON_LOADER_H"

    fout = open(path + "/json_loader.h", "w")
    fout.write(content)
    fout.close()
