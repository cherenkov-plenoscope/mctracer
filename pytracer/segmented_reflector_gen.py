import mctracer as T
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



class SegmetedReflectorGenerator(object):

    def __init__(self,
                focal_length=5.,
                facet_spacing=0.6,
                outer_diameter=5.,
                inner_diameter=0.3,
                hybrid_factor=1.):
    
        self.focal_length = focal_length
        self.facet_spacing = facet_spacing
        self.outer_diameter = outer_diameter
        self.inner_diameter = inner_diameter
        self.hybrid_factor = hybrid_factor

        self.optical_axis = unit_z
        self.facet_fill_factor = 0.99
        self.mirror_colour = T.Color(128, 128, 128)
        self.inner_mirror_colour = T.Color(20, 20, 20)

        limits = T.Limits(0., 1000.)
        con = T.Constant(0.5, limits)

        self.outer_mirror_reflection = con



        self.focal_point = self.optical_axis * self.focal_length
        self.facet_positions = make_hexgrid(
            outer_radius=self.outer_diameter/2. - self.facet_spacing/2.0,
            inner_radius=self.inner_diameter/2. + self.facet_spacing/2.0, 
            spacing=self.facet_spacing
        )
        self.facet_positions[:,2] = calc_facet_zpos(
            dist=np.linalg.norm(self.facet_positions, axis=1),
            focal_length=self.focal_length,
            hybrid_factor=self.hybrid_factor )
        #self.facet_positions = make_Vector3D(facet_positions)
        self.optimize_reflector_z_pos()
        self.rotation_axis, self.rot_angle = self.init_facet_orientations()
        self.facet_radius = self.init_facet_radius()

        self.init_facets()
        self.init_reflector()


    def __str__(self):
        buf = "SegmetedReflectorGenerator:\n"
        buf += "focal_length: " + self.focal_length + " m\n"
        buf += "number of facets: " + self.facet_positions + "\n"
        buf += "max Aperture diameter: " + self.outer_diameter + "\n"
        buf += "Hybrid: "
        buf += self.hybrid_factor + " Davies Cotton + "
        buf += 1.0 - self.hybrid_factor + " Parabolid\n"

        return buf
        
    def optimize_reflector_z_pos(self):
        deviation = 0.0
        iteration_conter = 0
        while True:
            deviation = self.focal_length - self.average_facet_distance()
            iteration_conter += 1

            self.facet_positions[:,2] -= deviation/2.
            
            if abs(deviation) <= self.focal_length*1e-6:
                break

            if iteration_conter > 100:
                raise Exception("Exceeded max number of 100 iterations. \n"
                            "Can not reach best reflector z position. ")            

    def average_facet_distance(self):
        return np.mean(
                        np.linalg.norm(
                            self.facet_positions - self.focal_point, axis=1
                            )
                        )

    def init_facet_orientations(self):
        focal_point_to_facet_pos = self.focal_point -  self.facet_positions
        rotation_axis = np.cross(focal_point_to_facet_pos, unit_z)

        c = dot(unit_z, focal_point_to_facet_pos.T)/norm(focal_point_to_facet_pos, axis=1) 
        angle = arccos(clip(c, -1, 1))
        rot_angle = -0.5 * angle

        return rotation_axis, rot_angle

    def init_facet_radius(self):
        facet_diameter_peak2peak = 2./math.sqrt(3.) * self.facet_spacing
        return  0.5*facet_diameter_peak2peak * self.facet_fill_factor


    def init_facets(self):

        self.facets = []

        for i, (f_pos, f_rot_ax, f_rot_angle) in enumerate(zip(
                self.facet_positions, 
                self.rotation_axis, 
                self.rot_angle)):

            facet = T.SphereCapWithHexagonalBound()
            pos_vector = T.Vector3D(*f_pos)
            rot_vector = T.Vector3D(*f_rot_ax)
            rot_object = T.Rotation3D(rot_vector, f_rot_angle)
            facet.set_name_pos_rot("mirror_ID_"+str(i), pos_vector, rot_object)
            
            facet.set_outer_color(self.mirror_colour);
            facet.set_inner_color(self.inner_mirror_colour);
            facet.set_outer_reflection(self.outer_mirror_reflection);
            facet.set_curvature_radius_and_outer_hex_radius(
                self.focal_length*2.0,
                self.facet_radius)

            self.facets.append(facet)


    def init_reflector(self):
        self.reflector = T.Frame("Reflector", T.Vector3D(5, 0, -0.5), T.Rotation3D.null)
        for facet in self.facets:
            self.reflector.set_mother_and_child(facet)

        self.reflector.cluster_using_helper_frames()



if __name__ == '__main__':
    world = T.Frame("World", T.Vector3D.null, T.Rotation3D.null)

    green = T.Color(0, 108, 30)
    floor = T.Plane('floor', T.Vector3D(0, 0, -2.5), T.Rotation3D.null)
    floor.set_inner_color(green)
    floor.set_outer_color(green)
    floor.set_x_y_width(1e6, 1e6)
    world.set_mother_and_child(floor)

    ref_gen = SegmetedReflectorGenerator(
                focal_length=5.,
                facet_spacing=0.6,
                outer_diameter=3.5,
                inner_diameter=0.,
                hybrid_factor=1.)

    ref = ref_gen.reflector
    world.set_mother_and_child(ref)


    world.init_tree_based_on_mother_child_relations()

    def start():
        propagation_settings = T.TracerSettings()
        cam = T.FreeOrbitCamera(world, propagation_settings)


    start()
    