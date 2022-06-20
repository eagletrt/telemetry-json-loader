import json
from os import sched_get_priority_max
from typing import List
from pathlib import Path
from json_loader.scripts.generators.cpp_generator import generate
from json_loader.scripts.utils.utils import *
from json_loader.scripts.utils.schema import Schema, SchemaModelItem, Struct

config_file = open("config.json")
config = json.loads(config_file.read())
config_file.close()


# lst = os.listdir(config["input_folder"])
# print(lst)
schemas: List[Schema] = []
out_paths: List[str] = []
names: List[str] = []
path = Path(config["input_folder"])
for el in path.iterdir():
    if(el.is_dir()):
        files = [x for x in el.iterdir() if x.is_file()]
        schema_models = []
        for file in files:
            f = open(file)
            js = json.loads(f.read())
            f.close()

            model_name = file.name.split(".")[0]
            schema_models.append(SchemaModelItem(model_name, js))

        schemas.append(Schema(schema_models))
        out_paths.append(config["cpp_out_path"])
        names.append(el.name)

all_structs: List[Struct]= []
for i, schema in enumerate(schemas):
    # if not i == 0:
    #     for strct in schema.structs[:]:
    #         for all_strct in all_structs:
    #             if strct == all_strct:
    #                 schema.structs.remove(strct)
    #                 print(f"Found duplicate {strct}")

    print(f"Generating {names[i]}")
    check_or_create_dir(out_paths[i])
    generate(names[i], Path(out_paths[i]), schema)

    # all_structs.extend(schema.structs)