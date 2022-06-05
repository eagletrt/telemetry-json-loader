from typing import List
from json_loader.scripts.utils.utils import vector_recursive_index

types_to_rapidjson = {
    "bool": "Bool",
    "int": "Int",
    "float": "Double",
    "std::string": "String",
    "std::vector": "Array",
}

class BasicField:
    def __init__(self, typename, name):
        self.typename = typename
        self.name = name
    def set_name(self, name: str)->None:
        self.name = name
    def get_rapidjson_type(self)->str:
        return types_to_rapidjson[self.typename]
    def __str__(self):
        return f"Basic {self.name}: {self.typename}"
class VectorField:
    def __init__(self, typename: str, recursion_level: int, name: str):
        self.typename = typename
        self.recursion_level = recursion_level
        self.name = name
    def set_name(self, name: str)->None:
        '''set_name'''
        self.name = name
    def set_type_name(self, typename: str)->None:
        '''set_type_name'''
        self.typename = typename
    def set_recursion_level(self, recursion_level: int):
        '''set_recursion_level'''
        self.recursion_level = recursion_level
    def get_cpp_serialize_loop(self)->str:
        '''get_cpp_serialize_loop'''
        ret = ""
        return ret
    def get_cpp_deserialize_loop(self) -> str:
        '''get_cpp_deserialize_loop'''
        ret = ""
        for i in range(self.recursion_level):
            index = "i" * (i+1)
            rec_index = vector_recursive_index("i", self.recursion_level-1, i)
            ret += "\t"*(i+1)+f"obj.{self.name}{rec_index}.resize(doc[\"{self.name}\"]{rec_index}.Size());\n"
            ret += "\t"*(i+1)+f"for(rapidjson::SizeType {index} = 0; {index} < doc[\"{self.name}\"]{rec_index}.Size(); {index}++)"+"{\n"

        rec_index = vector_recursive_index("i", self.recursion_level, self.recursion_level)
        if not isinstance(self.typename, Struct):
            ret += "\t"*(self.recursion_level+1) + f"obj.{self.name}{rec_index} = doc[\"{self.name}\"]{rec_index}.Get{self.typename.get_rapidjson_type()}();\n"
        else:
            ret += "\t"*(self.recursion_level+1) + f"Deserialize(obj.{self.name}{rec_index}, doc[\"{self.name}\"]{rec_index});\n"
        ret += "\t"+"}"*self.recursion_level
        return ret

    def get_cpp_type(self):
        cpp_t_i = "std::vector<"*self.recursion_level
        cpp_t_f = ">"*self.recursion_level
        return f"{cpp_t_i}{self.typename.typename}{cpp_t_f}"
    def get_rapidjson_type(self)->str:
        return types_to_rapidjson["std::vector"]
    def __str__(self):
        return f"Vector {self.name}: {self.recursion_level} * {self.typename.typename}"

class Struct:
    def __init__(self, typename: str, fields: list, is_main_struct = False,  name = ""):
        self.name = name
        self.typename = typename
        self.fields = fields
        self.is_main_struct = is_main_struct
    def add_field(self, field) -> None:
        self.fields.append(field)
    def __str__(self):
        s = f"\r\nStruct {self.is_main_struct} {self.typename}\n"
        for field in self.fields:
            s += "\t" + str(field) + "\n"
        return s[:-1]

map_types = {
    bool: BasicField("bool", ""),
    int:  BasicField("int", ""),
    float:BasicField("float", ""),
    str:  BasicField("std::string", ""),
    list: VectorField("std::vector", 0, ""),
    dict: Struct("", [])
}
def parse_dict(dct: dict)->list:
    ''' parse_dict '''
    new_fields = []
    for key, value in dct.items():
        inst = map_types[type(value)]
        if key == "ttt":
            print(inst)
        if isinstance(inst, BasicField):
            inst = BasicField(inst.typename, key)
            new_fields.append(inst)
        elif isinstance(inst, VectorField):
            curr = value
            recursion_value = 0
            while isinstance(curr, list):
                curr = curr[0]
                recursion_value += 1
            vec_type = map_types[type(curr)]
            if isinstance(vec_type, Struct):
                struct_fields = parse_dict(curr)
                vec_type = Struct(key+"_o", struct_fields)
            new_fields.append(VectorField(vec_type, recursion_value, key))
        elif isinstance(inst, Struct):
            new_strct = Struct(key+"_t", parse_dict(value), False, key)
            new_fields.append(new_strct)
    return new_fields

class SchemaModelItem:
    def __init__(self, name: str, json_model: dict):
        self.name = name
        self.json_model = json_model

class Schema:
    def __init__(self, schema_model_items: List[SchemaModelItem]):
        self.structs: List[Struct] = []
        for model in schema_model_items:
            new_struct = Struct(model.name, parse_dict(model.json_model), True)
            self.structs.append(new_struct)
        self.structs = self.extract_structs(self.structs)
        self.sort()

    def extract_structs(self, structs) -> List[Struct]:
        nested_structs: List[Struct] = []
        for struct in structs:
            if isinstance(struct, Struct):
                nested_structs.append(struct)
                new = self.extract_structs(struct.fields)
                if not new == []:
                    nested_structs.extend(new)
            elif isinstance(struct, VectorField):
                if isinstance(struct.typename, Struct):
                    nested_structs.append(struct.typename)
                    new = self.extract_structs(struct.typename.fields)
                    if not new == []:
                        nested_structs.extend(new)
        return nested_structs

    def sort(self):
        copy = self.structs
        self.structs = []
        for struct in copy:
            if not struct.is_main_struct:
                self.structs.insert(0, struct)
            else:
                self.structs.append(struct)

    def __str__(self):
        s = ""
        for struct in self.structs:
            s += str(struct) + "\n"
        return s[:-1]