#include "BiConvexLens.h"
//------------------------------------------------------------------------------
void BiConvexLens::set_focal_length_and_diameter(const double focal_length, const double diameter) {

	double offset = 0.5*
		height_of_a_cap_given_focal_length_and_diameter(focal_length, diameter);

	front_cap = create_cap_with_pos_rot_name_focal_lengt_and_diameter(
		Vector3D(0.0, 0.0, -offset),
		Rotation3D(0.0, 0.0, 0.0),
		name_of_mother_frame_plus("_front_cap"),
		focal_length,
		diameter
	);

	rear_cap = create_cap_with_pos_rot_name_focal_lengt_and_diameter(
		Vector3D(0.0, 0.0, +offset),
		Rotation3D(M_PI, 0.0, 0.0),
		name_of_mother_frame_plus("_rear_cap"),
		focal_length,
		diameter
	);

	this->set_mother_and_child(front_cap);
	this->set_mother_and_child(rear_cap);
}
//------------------------------------------------------------------------------
std::string BiConvexLens::name_of_mother_frame_plus(const std::string appendix)const {
	std::string front_cap_name = name_of_frame;
	return front_cap_name.append(appendix);
}
//------------------------------------------------------------------------------
double BiConvexLens::height_of_a_cap_given_focal_length_and_diameter(
	const double focal_length, const double diameter
)const {
	double curvature_radius = focal_length/2.0;
	double outer_radius = diameter/2.0;

	return curvature_radius - 
		sqrt(curvature_radius*curvature_radius - outer_radius*outer_radius);
}
//------------------------------------------------------------------------------
SphereCapWithCylinderBound* BiConvexLens::create_cap_with_pos_rot_name_focal_lengt_and_diameter(
	const Vector3D pos, 
	const Rotation3D rot, 
	const std::string name,
	const double focal_length, 
	const double diameter
)const {

	SphereCapWithCylinderBound* cap;
	cap = new SphereCapWithCylinderBound;
	cap->set_frame(name, pos, rot);

	if(has_inner_surface())
		cap->set_inner_surface(get_inner_surface());

	if(has_outer_surface())
		cap->set_outer_surface(get_outer_surface());

	if(has_inner_medium())
		cap->set_inner_medium(get_inner_medium());

	if(has_outer_medium())
		cap->set_outer_medium(get_outer_medium());

	cap->set_focal_length_and_cap_radius(focal_length/2.0, diameter/2.0);
	cap->set_allowed_frames_to_propagate_to(this);	

	return cap;
}
//------------------------------------------------------------------------------