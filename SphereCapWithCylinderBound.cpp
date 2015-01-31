#include "SphereCapWithCylinderBound.h"
//------------------------------------------------------------------------------
void SphereCapWithCylinderBound::set_focal_length_and_cap_radius(
	const double focal_length,
	const double cap_radius
) {
	curvature_radius = 2.0*focal_length;
	this->focal_length = focal_length;
	this->cap_radius = cap_radius;
	restrict_cap_radius_to_curvature_radius();
	CylBounds.set_radius(cap_radius);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void SphereCapWithCylinderBound::post_initialize_radius_of_enclosing_sphere() {
	
	double sphere_center_to_edge_in_z = 
		sqrt(curvature_radius*curvature_radius - cap_radius*cap_radius);

	radius_of_sphere_enclosing_all_children = hypot(
		cap_radius,
		curvature_radius - sphere_center_to_edge_in_z
	);	
}
//------------------------------------------------------------------------------
void SphereCapWithCylinderBound::restrict_cap_radius_to_curvature_radius() {
	if(cap_radius > curvature_radius)
		cap_radius = curvature_radius;
}
//------------------------------------------------------------------------------
void SphereCapWithCylinderBound::print()const {
	std::stringstream out;
	out << "SphereCapWithCylinderBound: " << name_of_frame << "\n";
	out << get_SphereCapCylinderBound_print();
	std::cout << out.str();
}
//------------------------------------------------------------------------------
std::string SphereCapWithCylinderBound::get_SphereCapCylinderBound_print()const {
	std::stringstream out;
	out << "focal length = " << focal_length << "m\n";
	out << "curvature radius = " << curvature_radius << "m\n";
	out << "cap radius = " << cap_radius << "m\n";
	out << "f/D = " << focal_length/(2.0*cap_radius) << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
Intersection* SphereCapWithCylinderBound::calculate_intersection_with(const Ray* ray)const {
	SphericalCapRayIntersectionEquation sphereCapRayEq(curvature_radius, ray);

	if(sphereCapRayEq.has_solutions()) {
		
		return get_causeal_intersection(
			&sphereCapRayEq,
			&CylBounds,
			ray
		);
	}

	return empty_intersection();
}