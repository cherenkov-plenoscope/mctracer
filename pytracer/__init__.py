import inspect
import mctracer

def print_get_print(self):
    return self.get_print()

cls_list = [x[1] for x in inspect.getmembers(mctracer, inspect.isclass) if not x[0][0] == '_']
for cls in cls_list:
    cls.__str__ = print_get_print
    #cls.__repr__ = print_get_print

from mctracer import * 

from segmented_reflector_gen import SegmetedReflectorGenerator