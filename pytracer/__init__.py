from __future__ import print_function
import inspect
import mctracer



global_colors = {}
global_frames = []
zero_reflection = mctracer.Constant(0., mctracer.Limits(200e-9, 1200e-9))
global_sensors = []
 
def print_get_print(self):
    return self.get_print()

inpspect_out = [x for x in inspect.getmembers(mctracer) if not 'cvar' in x[0]]
only_classes = [x for x in inpspect_out if inspect.isclass(x[1])]

cls_list = [x[1] for x in only_classes if not x[0][0] == '_']
for cls in cls_list:
    cls.__str__ = print_get_print
    #cls.__repr__ = print_get_print




__old_Frame__init__ = mctracer.Frame.__init__
def __new_Frame__init__(self, *k, **kw):
    __old_Frame__init__(self)

    name = kw.get('name', self.__class__.__name__)

    if 'pos' in kw:
        pos = mctracer.Vector3D(*kw['pos'])
    else:
        pos = mctracer.Vector3D.null

    if 'rot' in kw:
        if len(kw['rot']) == 3:
            rot = mctracer.Rotation3D(*kw['rot'])
        elif len(kw['rot']) == 2:
            axis, angle = kw['rot']
            rot_axis = mctracer.Vector3D(*axis)
            rot = mctracer.Rotation3D(rot_axis, angle)
        else:
            raise ValueError("Cannot make Rotation3D from:"+repr(kw['rot']))
    else:
        rot = mctracer.Rotation3D.null

    self.set_name_pos_rot(name, pos, rot)
mctracer.Frame.__init__ = __new_Frame__init__



#frame_kids = [x for x in inspect.getmembers(mctracer, inspect.isclass) if issubclass(x[1], mctracer.Frame)]
#surface_kids = [x for x in inspect.getmembers(mctracer, inspect.isclass) if issubclass(x[1], mctracer.SurfaceEntity)]
def make_color(c):
    if issubclass(c.__class__, mctracer.Color) or c is None:
        return c
    else:
        if len(c) == 3:
            c = tuple(c)
            if not c in global_colors:
                global_colors[c] = mctracer.Color(*c)
            return global_colors[c]
        else:
            raise ValueError("Cannot make Color from:"+repr(c))

def parse_color_parameter(c):
    """ return inner and outer color.

    cout be: 
        * Color
        * (R,G,B)
        * (Color, Color)
        * ((R,G,B), (R,G,B))
        * (None, Color)
        * (None, (R,G,B))
        * (Color, None)
        * ((R,G,B), None)
    """
    try:
        # first 2 cases
        c = make_color(c)
        return c, c
    except ValueError:
        c1, c2 = c
        return make_color(c1), make_color(c2)


__old_SurfaceEntity__init__ = mctracer.SurfaceEntity.__init__
def __new_SurfaceEntity__init__(self, *k, **kw):
    __old_SurfaceEntity__init__(self)
    super(mctracer.SurfaceEntity, self).__init__(*k, **kw)

    if 'inner_reflection' in kw:
        self.set_inner_reflection(kw['inner_reflection'])
    if 'outer_reflection' in kw:
        self.set_outer_reflection(kw['outer_reflection'])

    if 'inner_refraction' in kw:
        self.set_inner_refraction(kw['inner_refraction'])
    if 'outer_refraction' in kw:
        self.set_outer_refraction(kw['outer_refraction'])

    if 'inner_absorption' in kw:
        self.set_inner_refraction(kw['inner_absorption'])
    if 'outer_absorption' in kw:
        self.set_outer_refraction(kw['outer_absorption'])

    if 'color' in kw:
        c1, c2 = parse_color_parameter(kw['color'])
        self.set_inner_color(c1)
        self.set_outer_color(c2)

    is_sensor = kw.get('is_sensor', False)
    if is_sensor:
        global_sensors.append( 
            mctracer.PerfectSensor(len(global_sensors), self))  
mctracer.SurfaceEntity.__init__ = __new_SurfaceEntity__init__



__old_Plane__init__ = mctracer.Plane.__init__
def __new_Plane__init__(self, *k, **kw):
    __old_Plane__init__(self)
    super(mctracer.Plane, self).__init__(*k, **kw)
    
    shape = kw.get('shape', (1, 1))
    self.set_x_y_width(*shape)
mctracer.Plane.__init__ = __new_Plane__init__


__old_Disc__init__ = mctracer.Disc.__init__
def __new_Disc__init__(self, *k, **kw): 
    __old_Disc__init__(self)
    super(mctracer.Disc, self).__init__(*k, **kw)

    radius = kw.get('radius', 1.)
    self.set_radius(radius)
    
mctracer.Disc.__init__ = __new_Disc__init__


__old_Sphere__init__ = mctracer.Sphere.__init__
def __new_Sphere__init__(self, *k, **kw):
    __old_Sphere__init__(self)
    super(mctracer.Sphere, self).__init__(*k, **kw)
    radius = kw.get('radius', 1.)
    self.set_radius(radius)
mctracer.Sphere.__init__ = __new_Sphere__init__



__old_Cylinder__init__ = mctracer.Cylinder.__init__
def __new_Cylinder__init__(self, *k, **kw):
    __old_Cylinder__init__(self)
    super(mctracer.Cylinder, self).__init__(*k, **kw)
    radius = kw.get('radius', 1.)
    start = kw.get('start', (0,0,0))
    stop = kw.get('stop', (0,0,1))
    self.set_cylinder(radius, mctracer.Vector3D(*start), mctracer.Vector3D(*stop))
mctracer.Cylinder.__init__ = __new_Cylinder__init__


__old_Triangle__init__ = mctracer.Triangle.__init__
def __new_Triangle__init__(self, *k, **kw):
    __old_Triangle__init__(self)
    super(mctracer.Triangle, self).__init__(*k, **kw)

    corners=kw.get('corners', (0., 0., 1, 1, -1, -1))
    self.set_corners_in_xy_plane(*corners)
mctracer.Triangle.__init__ = __new_Triangle__init__


__old_SphereCapWithHexagonalBound__init__ = mctracer.SphereCapWithHexagonalBound.__init__
def __new_SphereCapWithHexagonalBound__init__(self, *k, **kw):
    __old_SphereCapWithHexagonalBound__init__(self)
    SurfaceEntity.__init__(self, *k, **kw)

    curvature_radius=kw.get('curvature_radius', 1.)
    outer_hex_radius=kw.get('outer_hex_radius', 1.)

    self.set_curvature_radius_and_outer_hex_radius(curvature_radius, outer_hex_radius)
mctracer.SphereCapWithHexagonalBound.__init__ = __new_SphereCapWithHexagonalBound__init__




def print_frame_dict(frame_dict):
    for name, value in frame_dict.iteritems():
        if isinstance(value, dict):
            print_frame_dict(value)
        else:
            print( repr(name), value.get_name() )


def set_names_frame_dict(frame_dict, parent_name=None):
    for name, value in frame_dict.items():
        if isinstance(value, dict):
            set_names_frame_dict(value, parent_name=name)
        else:
            if name:
                value.set_name(name)
            elif not parent_name is None:
                value.set_name(parent_name)
            else:
                pass

def set_mother_and_child_frame_dict(frame_dict):
    # each frame_dict must have one_key, which is the T.Frame.
    parent_frame = frame_dict['']
    for name, value in frame_dict.items():
        if not name:
            continue
        elif isinstance(value, dict):

            sub_parent = set_mother_and_child_frame_dict(value)
            print( "setting ", sub_parent.get_name(), "as child of", parent_frame.get_name())
            parent_frame.set_mother_and_child(sub_parent)
        else:
            print( "setting ", value.get_name(), "as child of", parent_frame.get_name())
            parent_frame.set_mother_and_child(value)
    return parent_frame



from mctracer import * 
from segmented_reflector_gen import SegmentedReflector