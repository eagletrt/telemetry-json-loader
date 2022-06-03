from ..utils import gen_types
from ..utils import templates

from .cpp_generator import cpp_generator
from .h_generator import h_generator

def generate(path, classes):
    cpp_generator(path, classes)
    h_generator(path, classes)
