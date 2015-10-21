#include "BiConvexLensHexBound.h"
//------------------------------------------------------------------------------
void BiConvexLensHexBound::set_curvature_radius_and_outer_hex_radius(
	const double curvature_radius,
	const double outer_radius
) {
	const double cap_hight = height_of_a_cap_given_curv_radius_and_outer_radius(
			curvature_radius, 
			outer_radius
		);

	front_cap = create_cap_with_pos_rot_name_focal_lengt_and_outer_radius(
		Vector3D(0.0, 0.0, -cap_hight),
		Rotation3D::null,
		name_of_frame + "_front_cap",
		curvature_radius,
		outer_radius
	);

	rear_cap = create_cap_with_pos_rot_name_focal_lengt_and_outer_radius(
		Vector3D(0.0, 0.0, +cap_hight),
		Rotation3D(M_PI, 0.0, 0.0),
		name_of_frame + "_rear_cap",
		curvature_radius,
		outer_radius
	);

	this->set_mother_and_child(front_cap);
	this->set_mother_and_child(rear_cap);
}
//------------------------------------------------------------------------------
double BiConvexLensHexBound::height_of_a_cap_given_curv_radius_and_outer_radius(
	const double curvature_radius, const double outer_radius
)const {

	return curvature_radius - 
		sqrt(curvature_radius*curvature_radius - outer_radius*outer_radius);
}
//------------------------------------------------------------------------------
SphereCapWithHexagonalBound* BiConvexLensHexBound::create_cap_with_pos_rot_name_focal_lengt_and_outer_radius(
	const Vector3D pos, 
	const Rotation3D rot, 
	const std::string name,
	const double curv_radius, 
	const double outer_radius
)const {

	SphereCapWithHexagonalBound* cap;
	cap = new SphereCapWithHexagonalBound;
	cap->set_name_pos_rot(name, pos, rot);
	cap->take_boundary_layer_properties_but_inside_out_from(this);
	cap->set_curvature_radius_and_outer_hex_radius(curv_radius, outer_radius);
	cap->set_allowed_frames_to_propagate_to(this);	

	return cap;
}
//------------------------------------------------------------------------------
