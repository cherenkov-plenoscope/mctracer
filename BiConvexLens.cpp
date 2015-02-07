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
	if(this->has_color())
		cap->set_color(this->get_color());
	if(this->does_refract())
		cap->set_refraction_properties(this->get_refraction_properties());
	cap->set_focal_length_and_cap_radius(focal_length/2.0, diameter/2.0);
	cap->set_allowed_frames_to_propagate_to(this);	

	return cap;
}
//------------------------------------------------------------------------------