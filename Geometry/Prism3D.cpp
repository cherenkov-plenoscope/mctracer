#include "Prism3D.h"
//------------------------------------------------------------------------------
void Prism3D::set_triangle_edge(const double edge_length) {
	
	double cap_hight = height_of_a_cap_given_curv_radius_and_outer_radius(
			curvature_radius, 
			aperture_radius
		);

	front_cap.set_name_pos_rot(
		name_of_frame + "_front_cap",
		Vec3(0.0, 0.0, -cap_hight),
		Rotation3D::null
	);
	front_cap.take_boundary_layer_properties_but_inside_out_from(this);
	front_cap.set_curvature_radius_and_outer_hex_radius(curvature_radius, aperture_radius);
	front_cap.set_allowed_frames_to_propagate_to(this);

	rear_cap.set_name_pos_rot(
		name_of_frame + "_rear_cap",
		Vec3(0.0, 0.0, cap_hight),
		Rotation3D(M_PI, 0.0, 0.0)
	);
	rear_cap.take_boundary_layer_properties_but_inside_out_from(this);
	rear_cap.set_curvature_radius_and_outer_hex_radius(curvature_radius, aperture_radius);
	rear_cap.set_allowed_frames_to_propagate_to(this);

	this->set_mother_and_child(&front_cap);
	this->set_mother_and_child(&rear_cap);
}