#include "BiConvexLensHexBound.h"
//------------------------------------------------------------------------------
void BiConvexLensHexBound::set_curvature_radius_and_outer_hex_radius(
    const double curvature_radius,
    const double outer_aperture_radius
) {
    double cap_hight = height_of_a_cap_given_curv_radius_and_outer_radius(
            curvature_radius, 
            outer_aperture_radius
        );

    front_cap.set_name_pos_rot(
        name_of_frame + "_front_cap",
        Vector3D(0.0, 0.0, -cap_hight),
        Rotation3D::null
    );
    front_cap.take_boundary_layer_properties_but_inside_out_from(this);
    front_cap.set_curvature_radius_and_outer_hex_radius(
        curvature_radius, outer_aperture_radius
    );
    front_cap.set_allowed_frames_to_propagate_to(this);

    rear_cap.set_name_pos_rot(
        name_of_frame + "_rear_cap",
        Vector3D(0.0, 0.0, cap_hight),
        Rotation3D(M_PI, 0.0, 0.0)
    );
    rear_cap.take_boundary_layer_properties_but_inside_out_from(this);
    rear_cap.set_curvature_radius_and_outer_hex_radius(
        curvature_radius, outer_aperture_radius
    );
    rear_cap.set_allowed_frames_to_propagate_to(this);

    this->set_mother_and_child(&front_cap);
    this->set_mother_and_child(&rear_cap);

    add_edge_faces(curvature_radius, outer_aperture_radius);
}
//------------------------------------------------------------------------------
void BiConvexLensHexBound::add_edge_faces(
    const double curvature_radius,
    const double outer_aperture_radius
) {
    
    double inner_aperture_radius = outer_aperture_radius*sqrt(3.0)/2.0;
    double hight = 2.0*(
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

    for(uint i=0; i<6; i++) {

        double phi = double(i)*1.0/3.0*M_PI;
        
        walls[i].set_name_pos_rot(
            "wall_" + std::to_string(i),
            Vector3D(
                inner_aperture_radius*sin(phi), 
                inner_aperture_radius*cos(phi), 
                0.0),
            Rotation3D(M_PI*0.5, M_PI*0.5, phi)
        );
        walls[i].set_x_hight_and_y_width(hight, outer_aperture_radius);
        walls[i].set_outer_color(&Color::green);
        walls[i].set_inner_color(&Color::green);
        this->set_mother_and_child(&walls[i]);
    }   
}
//------------------------------------------------------------------------------
double BiConvexLensHexBound::height_of_a_cap_given_curv_radius_and_outer_radius(
    const double curvature_radius, const double outer_radius
)const {
    return curvature_radius - 
        sqrt(curvature_radius*curvature_radius - outer_radius*outer_radius);
}