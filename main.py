import json
from pathlib import Path
from json_loader.scripts.generators.cpp_generator import generate
from json_loader.scripts.utils.utils import *
from json_loader.scripts.utils.schema import Schema, SchemaModelItem

config_file = open("config.json")
config = json.loads(config_file.read())
config_file.close()


schema_models = []
for filename in config["in_json_models"]:
    f = open(filename)
    js = json.loads(f.read())
    f.close()

    model_name = filename.split("/")[-1].split(".")[0]
    schema_models.append(SchemaModelItem(model_name, js))

schema = Schema(schema_models)
out_path = config["cpp_out_path"]
check_or_create_dir(out_path)
generate(config["library_name"], Path(out_path), schema)