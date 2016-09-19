import pytracer as T
from collections import defaultdict


zero = T.Constant(0., T.Limits(0., 200e-9))
leaf_reflection = T.Constant(0.5, T.Limits(200e-9, 1200e-9))
mirror_reflection = T.Constant(0.95, T.Limits(200e-9, 1200e-9))


world = {
  '': T.Frame(name="world"),
  'ground': T.Disc(pos=(0,0,-2), radius=1e6, color=(22,91,49), is_sensor=True),
  'tree' : 
  { '': T.Frame(pos=(5,5,-2)),
    'leaf_ball': T.Sphere(pos=(0,0,3), color=(25,200,0), radius=1.5, reflection=leaf_reflection),
    'pole' : T.Cylinder(color=(200,200,0), radius=0.25, stop=(0,0,1.5)),
  },
  'house': 
  { '': T.Frame(pos=(5,0,-2)),
    'wall1': T.Plane(pos=( 0,  1, 0.75), rot=(0, 1.57, 1.57), color=(180,180,180), shape=(1.5, 2.)),
    'wall2': T.Plane(pos=( 0, -1, 0.75), rot=(0, 1.57, 1.57), color=(180,180,180), shape=(1.5, 2.)),
    'wall3': T.Plane(pos=( 1,  0, 0.75), rot=(0, 1.57, 0.), color=(180,180,180), shape=(1.5, 2.)),
    'wall4': T.Plane(pos=(-1,  0, 0.75), rot=(0, 1.57, 0.), color=(180,180,180), shape=(1.5, 2.)),
    'upper_wall1': T.Triangle(pos=( 1, 0, 1.5), rot=(-1.57,0.0,-1.57), color=(180,180,180), corners=(-1, 0., +1., 0., 0., 1.)),
    'upper_wall2': T.Triangle(pos=(-1, 0, 1.5), rot=(-1.57,0.0,-1.57), color=(180,180,180), corners=(-1, 0., +1., 0., 0., 1.)),
    'roof' : 
    { '' : T.Frame(pos=(0, 0, 1.1)),
      'roof_part1': T.Plane(pos=(0,  0.707, 0.707), rot=(0, 0.7853,  1.57), color=(190,0,80), shape=(2., 2.2)),
      'roof_part2': T.Plane(pos=(0, -0.707, 0.707), rot=(0, 0.7853, -1.57), color=(190,0,80), shape=(2., 2.2)),
      'chimney': 
      { '' : T.Frame(pos=(0, 0.6, 2.1)),
        'chimney_wall1': T.Plane(pos=(0.15,0,0), rot=(0,1.57,0.0), color=(180,180,180), shape=(0.65, 0.3)),
        'chimney_wall2': T.Plane(pos=(-0.15,0,0), rot=(0,1.57,0), color=(180,180,180), shape=(0.65, 0.3)),
        'chimney_wall3': T.Plane(pos=(0,0.15,0), rot=(0,1.57,1.57), color=(180,180,180), shape=(0.65, 0.3)),
        'chimney_wall4': T.Plane(pos=(0,-0.15,0), rot=(0,1.57,1.57), color=(180,180,180), shape=(0.65, 0.3)),
      },
    },
  },
  'FACT' : 
  { '' : T.Frame(rot=(0,1.2,0)),
    "reflector": T.SegmentedReflector(name="",
                  reflection=mirror_reflection,
                  focal_length=4.889,
                  facet_spacing=0.62,
                  outer_diameter=4.2, 
                  inner_diameter=0.5, 
                  hybrid_factor=1.0),
    'GAPD_camera':
    { '': T.Frame(pos=(0, 0, 4.8892)),
      'front_cap': T.Disc(pos=(0, 0, 0.01), color=(230, 230, 215), radius=0.33),
      'back_cap': T.Disc(pos=(0, 0, 0.89), color=(230, 230, 215), radius=0.33), 
      'GAPD_sensor_plane': T.Disc(color=(0,255,128), radius=0.195, is_sensor=True),
      'body': T.Cylinder(color=(230, 230, 215), radius=0.33, stop=(0,0,0.89)),
    },
    'small_support_tubes':
    { '':T.Frame(pos=(0,0,2.7)),
      'support_tube1': T.Cylinder(start=(-0.83,+0.57,0.0), stop=(+0.83,+0.57,0.0), color=(75,75,75), radius=(0.015)),
      'support_tube2': T.Cylinder(start=(-0.83,-0.57,0.0), stop=(+0.83,-0.57,0.0), color=(75,75,75), radius=(0.015)),
      'support_tube3': T.Cylinder(start=(+0.83,+0.57,0.0), stop=(+0.83,-0.57,0.0), color=(75,75,75), radius=(0.015)),
      'support_tube4': T.Cylinder(start=(-0.83,-0.57,0.0), stop=(-0.83,+0.57,0.0), color=(75,75,75), radius=(0.015)),
    },
  },
}


T.set_names_frame_dict(world)
tree_node = T.set_mother_and_child_frame_dict(world)
tree_node.init_tree_based_on_mother_child_relations()

texture = T.Texture('./scenery/chile_night_sky.tif')
propagation_settings = T.PropagationConfig()
propagation_settings.set_sky_dome(texture)
cam = T.FreeOrbitCamera(tree_node, propagation_settings)
    