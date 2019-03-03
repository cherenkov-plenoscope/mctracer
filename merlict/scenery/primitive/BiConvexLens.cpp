// Copyright 2014 Sebastian A. Mueller
#include "merlict/scenery/primitive/BiConvexLens.h"
#include <math.h>

namespace merlict {

void BiConvexLens::set_curvature_radius_and_aperture_radius(
    const double curvature_radius,
    const double aperture_radius
) {
    SphereCapWithCylinderBound* front_cap =
        add<SphereCapWithCylinderBound>();
    SphereCapWithCylinderBound* rear_cap =
        add<SphereCapWithCylinderBound>();
    double cap_hight = height_of_a_cap_given_curv_radius_and_aperture_radius(
            curvature_radius,
            aperture_radius);
    front_cap->set_name_pos_rot(
        "front_cap",
        Vec3(0.0, 0.0, -cap_hight),
        ROT3_UNITY);
    front_cap->take_boundary_layer_properties_but_inside_out_from(this);
    front_cap->set_curvature_radius_and_outer_radius(
        curvature_radius,
        aperture_radius);
    front_cap->set_allowed_frames_to_propagate_to(this);
    rear_cap->set_name_pos_rot(
        "rear_cap",
        Vec3(0.0, 0.0, cap_hight),
        Rot3(M_PI, 0.0, 0.0));
    rear_cap->take_boundary_layer_properties_but_inside_out_from(this);
    rear_cap->set_curvature_radius_and_outer_radius(
        curvature_radius,
        aperture_radius);
    rear_cap->set_allowed_frames_to_propagate_to(this);
}

double BiConvexLens::height_of_a_cap_given_curv_radius_and_aperture_radius(
    const double curvature_radius, const double aperture_radius
)const {
    return curvature_radius - sqrt(
            curvature_radius*curvature_radius -
            aperture_radius*aperture_radius);
}

}  // namespace merlict
