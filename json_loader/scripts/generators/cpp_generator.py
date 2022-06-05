import jinja2 as j2
from pathlib import Path

from json_loader.scripts.utils.utils import vector_recursive_index
from json_loader.scripts.utils.schema import BasicField, Schema, Struct, VectorField

BASE_DIR = Path(__file__).parent

CPP_TMPLT = j2.Template((BASE_DIR / "templates/cpp.j2").read_text())
H_TMPLT = j2.Template((BASE_DIR / "templates/h.j2").read_text())

def generate(path: Path, schema: Schema)->None:
    '''generate'''
    cpp_path = path / "json_loader.cpp"
    cpp_path.write_text(CPP_TMPLT.render(
        schema=schema,
        isinstance=isinstance,
        Struct=Struct,
        Vector=VectorField,
        BasicField=BasicField,
        vec_rec_idx=vector_recursive_index
    ))

    h_path = path / "json_loader.h"
    h_path.write_text(H_TMPLT.render(
        schema=schema,
        isinstance=isinstance,
        Struct=Struct,
        Vector=VectorField,
        BasicField=BasicField
    ))
    