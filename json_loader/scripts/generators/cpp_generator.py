import jinja2 as j2
from pathlib import Path

from json_loader.scripts.utils.utils import vector_recursive_index
from json_loader.scripts.utils.schema import BasicField, Schema, Struct, VectorField

BASE_DIR = Path(__file__).parent

H_TMPLT = j2.Template((BASE_DIR / "templates/h.j2").read_text())

def generate(library_name: str, path: Path, schema: Schema)->None:
    '''generate'''
    h_path = path / (library_name + ".h")
    h_path.write_text(H_TMPLT.render(
        schema=schema,
        library_name=library_name.upper(),
        isinstance=isinstance,
        Struct=Struct,
        Vector=VectorField,
        BasicField=BasicField,
        vec_rec_idx=vector_recursive_index
    ))
    