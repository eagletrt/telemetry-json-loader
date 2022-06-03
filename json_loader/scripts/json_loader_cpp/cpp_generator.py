from ..utils import gen_types
from ..utils import templates
from ..utils import type_model
from ..utils.utils import *

def vector_recursive_index(index, max_count, count):
    if count == 0:
        return ""
    else:
        ret = "["+index*(max_count-count+1)+"]"
        ret += vector_recursive_index(index, max_count, count-1)
    return ret

def name_or_typename(field):
    return field.name if field.typename == "" else field.typename

def gen_deserialize(class_: templates.ClassTemplate):
    ret = ""
    if class_ == None:
        ret += "template <class T>\n"
        ret += "void Deserialize(T& obj, const json& j)\n"
        ret += "{\n"
        ret += "\tthrow std::logic_error(\"Deserialize function is not implemented for this struct\");\n"
        ret += "}\n"
        return ret
    ret += "template <>\n"
    ret += "void Deserialize({}& obj,const json& j)\n".format(name_or_typename(class_))
    ret += "{\n"
    ret += "\tCheckJson<{}>(obj, j);\n".format(name_or_typename(class_))
    for field in class_.content:
        f = field.name
        t = field.typename
        ret += "\tif(j.contains(\"{}\"))\n\t".format(f)
        ret += "{\n"

        if f == "array" or f == "array of objects":
            print(field)

        if isinstance(field, type_model.VectorType):
            ret += "\t\tobj.{}.clear();\n".format(f)
            last_type, count = field.get_vector_recursion()
            for i in range(count):
                prev_index = "i" * i
                vec_idx = vector_recursive_index("i", i, i)
                index = "i" * (i+1)

                if i < count -1 :
                    ret += "\t\t"*(i+1)+"int {} = 0;\n".format("k"*(i+1))
                    obj_idx = vector_recursive_index("k", i, i)
                    ret += "\t\t"*(i+1)+"obj.{} {}.push_back(".format(f, obj_idx)+"{});\n"

                if i == 0:
                    ret += "\t\t"*(i+1)+"for(auto "+"{}: j[\"{}\"])\n".format(index, f)
                else:
                    ret += "\t\t"*(i+1)+"for(auto "+"{}: {})\n".format(index, f"{prev_index}")

                ret += "\t\t"*(i+1)+"{\n"


            obj_idx = vector_recursive_index("k", count-1, count-1)
            if isinstance(last_type, type_model.BasicType):
                ret += "\t\t"*(count+1)+"obj.{} {}.push_back({});\n".format(f, obj_idx, "i"*count)
            else:
                ret += "\t\t"*(count+1)+"{} tmp;\n".format(name_or_typename(last_type))
                ret += "\t\t"*(count+1)+"Deserialize<{}>(tmp, {});\n".format(name_or_typename(last_type), "i"*count)
                ret += "\t\t"*(count+1)+"obj.{} {}.push_back(tmp);\n".format(f, obj_idx)
            for i in range(count):
                if i > 1:
                    ret += "\t\t"*((count+1)-i) + "{} ++;\n".format("k"*((count)-i))
                ret += "\t\t"*((count)-i) + "}\n"

        elif isinstance(field, type_model.BasicType):
            ret += "\t\tobj.{} = j[\"{}\"];\n".format(f, f)
        else:
            ret += "\t\tDeserialize<{}>(obj.{}, j[\"{}\"]);\n".format(t, f, f)

        ret += "\t}\n"
        
    ret += "}\n"
    return ret

def gen_serialize(class_: templates.ClassTemplate):
    ret = ""
    if class_ == None:
        ret += "template <class T>\n"
        ret += "json Serialize(const T& obj)\n"
        ret += "{\n"
        ret += "\tthrow std::logic_error(\"Serialize function is not implemented for this struct\");\n"
        ret += "}\n"
        return ret
    ret += "template <>\n"
    ret += "json Serialize(const {}& obj) \n".format(name_or_typename(class_))
    ret += "{\n"
    ret += "\tjson j;\n"
    for field in class_.content:
        f = field.name
        t = field.typename
        if isinstance(field, type_model.VectorType):
            last_type, count = field.get_vector_recursion()

            ret += "\tjson {}_json;\n".format(f)


            for i in range(count):
                vec_idx = vector_recursive_index("i", i, i)
                index = "i" * (i+1)
                ret += "\t"*(i+1)+"for(int {} = 0; {} < obj.{} {}.size(); {}++)\n".format(index, index, f, vec_idx, index)
                ret += "\t"*(i+1)+"{\n"

            if isinstance(last_type, type_model.BasicType):
                json_idx = vector_recursive_index("i", count-1, count-1)
                vec_idx = vector_recursive_index("i", count, count)
                ret += "\t"*(count+1)+"{}_json{}.push_back(obj.{} {});\n".format(f, json_idx, f, vec_idx)
            else:
                json_idx = vector_recursive_index("i", count-1, count-1)
                vec_idx = vector_recursive_index("i", count, count)
                ret += "\t"*(count+1)+"{}_json{}.push_back(Serialize<{}>(obj.{} {}));\n".format(f, json_idx, name_or_typename(last_type), f, vec_idx)

            for i in range(count):
                ret += "\t"*(count-i) + "}\n"
            ret += "\tj[\"{}\"] = {}_json;\n".format(f, f)

        elif gen_types.is_cpp_type(t):
            ret += "\tj[\"{}\"] = obj.{};\n".format(f, f)
        else:
            ret += "\tj[\"{}\"] = Serialize<{}>(obj.{});\n".format(f, t, f)
    ret += "\treturn j;\n"
    ret += "}\n"
    return ret

def gen_check(class_: type_model.Type):
    ret = ""
    if class_ == None:
        ret += "template <class T>\n"
        ret += "void CheckJson(const T& obj, const json& j)\n"
        ret += "{\n"
        ret += "\tthrow std::logic_error(\"Check function is not implemented for this struct\");\n"
        ret += "}\n"
        return ret

    ret += "template <>\n"
    ret += "void CheckJson(const {}& obj, const json& j)\n".format(name_or_typename(class_))
    ret += "{\n"
    for field in class_.content:
        f = field.name
        t = field.typename
        ret += "\tif(!j.contains(\"{}\"))\n".format(f)
        ret += "\t\tstd::cout << \"ERROR \" << "
        ret += "\"JSON does not contain key [{}] of type [{}] in object [{}]\"".format(f, t, class_.name)
        ret += " << std::endl;\n"
    ret += "}\n"
    return ret

def gen_load(class_: templates.ClassTemplate):
    ret = ""
    if class_ == None:
        ret += "template <class T>\n"
        ret += "bool LoadJson(T& obj, const std::string& path)\n"
        ret += "{\n"
        ret += "\tthrow std::logic_error(\"LoadJson function is not implemented for this struct\");\n"
        ret += "}\n"
        return ret

    ret += "template <>\n"
    ret += "bool LoadJson({}& obj, const std::string& path)\n".format(name_or_typename(class_))
    ret += "{\n"
    ret += "\tstd::ifstream f(path);\n"
    ret += "\tif(!f.is_open())\n"
    ret += "\t\treturn false;\n"
    ret += "\tjson j;\n"
    ret += "\tf >> j;\n"
    ret += "\tf.close();\n"
    ret += "\tDeserialize<{}>(obj, j);\n".format(name_or_typename(class_))
    ret += "\treturn true;\n"
    ret += "}\n"
    return ret


def gen_save(class_: templates.ClassTemplate):
    ret = ""
    if class_ == None:
        ret += "template <class T>\n"
        ret += "void SaveJson(const T& obj, const std::string& path)\n"
        ret += "{\n"
        ret += "\tthrow std::logic_error(\"SaveJson function is not implemented for this struct\");\n"
        ret += "}\n"
        return ret

    ret += "template <>\n"
    ret += "void SaveJson(const {}& obj, const std::string& path)\n".format(name_or_typename(class_))
    ret += "{\n"
    ret += "\tstd::ofstream f(path);\n"
    ret += "\tf << std::setw(4) << Serialize<{}>(obj);\n".format(name_or_typename(class_))
    ret += "\tf.close();\n"
    ret += "}\n"
    return ret


def gen_struct_fun(class_: templates.ClassTemplate):
    ret = ""
    ret += gen_check(class_)
    ret += gen_deserialize(class_)
    ret += gen_serialize(class_)
    ret += gen_load(class_)
    ret += gen_save(class_)
    return ret

def cpp_generator(path, classes):
    content = ""
    content += "#include \"json_loader.h\"\n\n"

    # Generate empty templates
    content += gen_check(None)
    content += gen_deserialize(None)
    content += gen_serialize(None)
    content += gen_load(None)
    content += gen_save(None)

    content += "\n\n"
    content += ("//"*20+"\n")*4
    content += "\n\n"

    # Generate implemented templates
    for class_ in classes:
        content += gen_struct_fun(class_)
        content += "\n\n"
        content += ("//"*20+"\n")*2
        content += "\n\n"


    fout = open(path + "/json_loader.cpp", "w")
    fout.write(content)
    fout.close()
