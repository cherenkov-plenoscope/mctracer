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
void SphereCapWithCylinderBound::set_curvature_radius_and_outer_radius(
		const double _curvature_radius,
		const double cap_radius
) {
	curvature_radius =_curvature_radius;
	this->focal_length = curvature_radius*0.5;
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
std::string SphereCapWithCylinderBound::get_print()const {
	std::stringstream out;
	out << SurfaceEntity::get_print();

	out << "SphereCapWithCylinderBound:\n";
	out << "| focal length: " << focal_length << "m\n";
	out << "| curvature radius: " << curvature_radius << "m\n";
	out << "| cap radius: " << cap_radius << "m\n";
	out << "| f/D: " << focal_length/(2.0*cap_radius) << "\n";
	return out.str();
}
//------------------------------------------------------------------------------
void SphereCapWithCylinderBound::calculate_intersection_with(
    const Ray* ray, 
    vector<const Intersection*> *intersections
)const {

	SphericalCapRayIntersectionEquation sphereCapRayEq(curvature_radius, ray);

	if(sphereCapRayEq.has_solutions()) {
		
		add_causeal_intersection(
			&sphereCapRayEq,
			&CylBounds,
			ray,
			intersections
		);
	}
}