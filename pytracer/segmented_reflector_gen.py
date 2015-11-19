import pytracer as T
import math
import numpy as np
from functools import partial
import collections

from numpy import dot, arccos, cross, clip
from numpy.linalg import norm

unit_x = np.array([1., 0., 0.])
unit_y = np.array([0., 1., 0.])
unit_z = np.array([0., 0., 1.])

def Vector3D_to_numpy(input):
    """ input can be a single Vector3D or a list of them.
    """
    if isinstance(input, collections.Iterable):
        N = len(input)
        out = np.zeros((N, 3), dtype=np.float64)
        for i, vec in enumerate(input):
            out[i, 0] = vec.x()
            out[i, 1] = vec.y()
            out[i, 2] = vec.z()
    else:
        vec = input
        out = np.zeros(3, dtype=np.float64)
        out[0] = vec.x()
        out[1] = vec.y()
        out[2] = vec.z()
    return out

def make_Vector3D(a):
    a = np.atleast_2d(a)
    return [T.Vector3D(*x) for x in a]

def make_hexgrid(outer_radius, spacing, inner_radius=0.):

    unit_hex_b = unit_y * spacing
    unit_hex_a = (unit_y * spacing/2. 
                + unit_x * spacing * np.sqrt(3.)/2.)

    sample_radius = int(2*math.floor(outer_radius / spacing))

    aa , bb  = np.mgrid[
        -sample_radius:sample_radius+1,
        -sample_radius:sample_radius+1,
    ]
    cell_ab = (unit_hex_a * aa[..., np.newaxis] 
             + unit_hex_b * bb[..., np.newaxis]
             )
    norms = np.linalg.norm(cell_ab, axis=2) 
    mask = (norms <= outer_radius) & (norms >= inner_radius)

    inside_cells = cell_ab[mask]

    return inside_cells

def make_hexgrid_Vector3D(outer_radius, spacing, inner_radius=0.):
    return make_Vector3D(make_hexgrid(outer_radius, spacing, inner_radius))

def calc_facet_zpos(dist, focal_length, hybrid_factor):
    f = focal_length
    h = hybrid_factor
    z_Davies_Cotton = f - np.sqrt( f**2. - dist**2.)
    z_parabolic = 1./4.*f * dist**2.

    return h * z_Davies_Cotton + (1-h) * z_parabolic


def optimize_reflector_z_pos(facet_positions, focal_length, focal_point):
    deviation = 0.0
    iteration_conter = 0
    f = focal_length
    while True:
        deviation = f - average_facet_distance(facet_positions, focal_point)
        iteration_conter += 1

        facet_positions[:,2] -= deviation/2.
        
        if abs(deviation) <= f*1e-6:
            break

        if iteration_conter > 100:
            raise Exception("Exceeded max number of 100 iterations. \n"
                        "Can not reach best reflector z position. ")            


def average_facet_distance(facet_positions, focal_point):
    return np.mean(
                    np.linalg.norm(
                        facet_positions - focal_point, axis=1
                        )
                    )

def init_facet_orientations(facet_positions, focal_point):
    focal_point_to_facet_pos = focal_point -  facet_positions
    rotation_axis = np.cross(focal_point_to_facet_pos, unit_z)

    c = dot(unit_z, focal_point_to_facet_pos.T)/norm(focal_point_to_facet_pos, axis=1) 
    angle = arccos(clip(c, -1, 1))
    rot_angle = -0.5 * angle

    return rotation_axis, rot_angle

def init_facet_radius(facet_spacing, facet_fill_factor):
    facet_diameter_peak2peak = 2./math.sqrt(3.) * facet_spacing
    return  0.5*facet_diameter_peak2peak * facet_fill_factor

def SegmentedReflector(name, reflection,
                pos=(0,0,0),
                rot=(0,0,0),
                focal_length=5.,
                facet_spacing=0.6,
                outer_diameter=5.,
                inner_diameter=0.3,
                hybrid_factor=1.,
                mirror_color=(128, 128, 128),
                inner_mirror_color=(20, 20, 20),
                ):

        optical_axis = unit_z
        facet_fill_factor = 0.99
        mirror_color = T.make_color(mirror_color)
        inner_mirror_color = T.make_color(inner_mirror_color)



        focal_point = optical_axis * focal_length
        facet_positions = make_hexgrid(
            outer_radius=outer_diameter/2. - facet_spacing/2.0,
            inner_radius=inner_diameter/2. + facet_spacing/2.0, 
            spacing=facet_spacing
        )

        facet_positions[:,2] = calc_facet_zpos(
            dist=np.linalg.norm(facet_positions, axis=1),
            focal_length=focal_length,
            hybrid_factor=hybrid_factor )

        optimize_reflector_z_pos(
            facet_positions, 
            focal_length, 
            focal_point)
            
        rotation_axis, rot_angle = init_facet_orientations(facet_positions, focal_point)
        facet_radius = init_facet_radius(facet_spacing, facet_fill_factor)

        reflector = T.Frame(name, T.Vector3D(*pos), T.Rotation3D(*rot))
        facets = []
        for i, (f_pos, f_rot_ax, f_rot_angle) in enumerate(zip(
                                                facet_positions, 
                                                rotation_axis, 
                                                rot_angle)):
            
            facet = T.SphereCapWithHexagonalBound(
                name="mirror_ID_"+str(i),
                pos=f_pos,
                rot=(f_rot_ax, f_rot_angle),
                color=(inner_mirror_color, mirror_color),
                outer_reflection=reflection,
                curvature_radius=focal_length*2.0,
                outer_hex_radius=facet_radius
                )
            
            facets.append( facet )
            reflector.set_mother_and_child(facet)
        
        reflector.cluster_using_helper_frames()
        reflector.___facets_ownership_list = facets

        return reflector
        


if __name__ == '__main__':
    world = T.Frame("World", T.Vector3D.null, T.Rotation3D.null)

    green = T.Color(0, 108, 30)
    floor = T.Plane('floor', T.Vector3D(0, 0, -2.5), T.Rotation3D.null)
    floor.set_inner_color(green)
    floor.set_outer_color(green)
    floor.set_x_y_width(1e6, 1e6)
    world.set_mother_and_child(floor)

    mirror_reflection = T.Constant(0.95, T.Limits(200e-9, 1200e-9))

    reflector = SegmentedReflector("reflector", mirror_reflection,
                focal_length=5.,
                facet_spacing=0.6,
                outer_diameter=3.5,
                inner_diameter=0.,
                hybrid_factor=1.)

    
    world.set_mother_and_child(reflector)
    world.init_tree_based_on_mother_child_relations()

    def start():
        texture = T.Texture('./scenery/chile_night_sky.tif')
        propagation_settings = T.TracerSettings()
        propagation_settings.set_sky_dome(texture)
        cam = T.FreeOrbitCamera(world, propagation_settings)


    start()
    