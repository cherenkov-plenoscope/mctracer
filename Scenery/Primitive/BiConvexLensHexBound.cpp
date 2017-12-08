#include "BiConvexLensHexBound.h"
#include "math.h"
//------------------------------------------------------------------------------
void BiConvexLensHexBound::set_curvature_radius_and_outer_hex_radius(
    const double curvature_radius,
    const double outer_aperture_radius
) {

    SphereCapWithHexagonalBound* front_cap = append<SphereCapWithHexagonalBound>();
    SphereCapWithHexagonalBound* rear_cap = append<SphereCapWithHexagonalBound>();

    double cap_hight = height_of_a_cap_given_curv_radius_and_outer_radius(
            curvature_radius, 
            outer_aperture_radius
        );

    front_cap->set_name_pos_rot(
        "front_cap",
        Vec3(0.0, 0.0, -cap_hight),
        Rot3::null
    );
    front_cap->take_boundary_layer_properties_but_inside_out_from(this);
    front_cap->set_curvature_radius_and_outer_hex_radius(
        curvature_radius, outer_aperture_radius
    );
    front_cap->set_allowed_frames_to_propagate_to(this);

    rear_cap->set_name_pos_rot(
        "rear_cap",
        Vec3(0.0, 0.0, cap_hight),
        Rot3(M_PI, 0.0, 0.0)
    );
    rear_cap->take_boundary_layer_properties_but_inside_out_from(this);
    rear_cap->set_curvature_radius_and_outer_hex_radius(
        curvature_radius, outer_aperture_radius
    );
    rear_cap->set_allowed_frames_to_propagate_to(this);

    const double inner_aperture_radius = outer_aperture_radius*sqrt(3.0)/2.0;
    const double hight = 2.0*(
        height_of_a_cap_given_curv_radius_and_outer_radius(
            curvature_radius, outer_aperture_radius
        )
        +
        sqrt(
            curvature_radius*curvature_radius -
            inner_aperture_radius*inner_aperture_radius
        )
        -
        curvature_radius
    );

    PlaneDualSphericalBound* walls[6];
    for(unsigned int i=0; i<6; i++) {
        const double phi = double(i)*1.0/3.0*M_PI;

        walls[i] = append<PlaneDualSphericalBound>();
        walls[i]->set_name_pos_rot(
            "wall_" + std::to_string(i),
            Vec3(
                inner_aperture_radius*sin(phi), 
                inner_aperture_radius*cos(phi), 
                0.0),
            Rot3(M_PI*0.5, M_PI*0.5, phi)
        );
        walls[i]->set_x_hight_and_y_width(hight, outer_aperture_radius);
        walls[i]->set_outer_color(&Color::GREEN);
        walls[i]->set_inner_color(&Color::GREEN);
    }   
}
//------------------------------------------------------------------------------
double BiConvexLensHexBound::height_of_a_cap_given_curv_radius_and_outer_radius(
    const double curvature_radius, const double outer_radius
)const {
    return curvature_radius - 
        sqrt(curvature_radius*curvature_radius - outer_radius*outer_radius);
}