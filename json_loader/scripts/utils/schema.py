from typing import List

class BasicField:
    def __init__(self, typename, name):
        self.typename = typename
        self.name = name
    def set_name(self, name):
        self.name = name
    def __str__(self):
        return f"Basic {self.name}: {self.typename}"
class VectorField:
    def __init__(self, typename: str, recursion_level: int, name: str):
        self.typename = typename
        self.recursion_level = recursion_level
        self.name = name
    def set_name(self, name: str):
        self.name = name
    def set_type_name(self, typename: str):
        self.typename = typename
    def set_recursion_level(self, recursion_level: int):
        self.recursion_level = recursion_level
    def get_cpp_type(self):
        cpp_t_i = "std::vector<"*self.recursion_level
        cpp_t_f = ">"*self.recursion_level
        return f"{cpp_t_i}{self.typename.typename}{cpp_t_f}"
    def __str__(self):
        return f"Vector {self.name}: {self.recursion_level} * {self.typename.typename}"

class Struct:
    def __init__(self, typename: str, fields: list, is_main_struct = False):
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

def parse_dict(dct: dict):
    new_fields = []
    for key, value in dct.items():
        inst = map_types[type(value)]
        if isinstance(inst, BasicField):
            inst = BasicField(inst.typename, key)
            new_fields.append(inst)
        elif isinstance(inst, VectorField):
            curr = value
            recursion_value = 0
            while type(curr) == list:
                curr = curr[0]
                recursion_value += 1
            vec_type = map_types[type(curr)]
            if isinstance(vec_type, Struct):
                struct_fields = parse_dict(curr)
                vec_type = Struct(key+"_o", struct_fields)
            new_fields.append(VectorField(vec_type, recursion_value, key))
    return new_fields

map_types = {
    bool: BasicField("bool", ""),
    int:  BasicField("int", ""),
    float:BasicField("float", ""),
    str:  BasicField("std::string", ""),
    list: VectorField("std::vector", 0, ""),
    dict: Struct("", [])
}

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