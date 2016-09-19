# coding: utf-8
from mctracer import *
import os.path

import sys

def trace(frame, event, arg):
    print "%s, %s:%d" % (event, frame.f_code.co_filename, frame.f_lineno)
    return trace

#sys.settrace(trace)

world= Frame("World", Vec3.null, Rot3.null)
tree= Frame("My_Tree", Vec3(5.0, 0.0, 0.0), Rot3.null)



leaf_ball = Sphere("leaf_ball", Vec3(0.0, 0.0, 2.0), Rot3.null)
leaf_ball.set_outer_color(Color(0, 128, 0))
leaf_ball.set_radius(0.5)


tree_pole = Cylinder("tree_pole", Vec3(0.0, 0.0, 0.5), Rot3.null)
tree_pole.set_outer_color(Color(64, 64, 0))
tree_pole.set_radius_and_length(0.1, 1.0)

tree.set_mother_and_child(leaf_ball)
tree.set_mother_and_child(tree_pole) 

house = Frame("house", Vec3(0.0, -5.0, 0.0), Rot3.null)

foundation_gray = Color(32, 32, 32)

foundation = Plane("foundation", Vec3(0.0, 0.0, 0.01), Rot3.null)
foundation.set_outer_color(foundation_gray)
foundation.set_inner_color(foundation_gray)
foundation.set_x_y_width(4.5, 4.5)

front_wall = Plane("wall1", Vec3(0.0, 0.0, 0.0), Rot3.null)

world.set_mother_and_child(tree)

world.init_tree_based_on_mother_child_relations()


propagation_settings = PropagationConfig()
cam =FreeOrbitCamera(world, propagation_settings)
