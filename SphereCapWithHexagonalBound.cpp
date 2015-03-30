#include "SphereCapWithHexagonalBound.h"
//------------------------------------------------------------------------------
void SphereCapWithHexagonalBound::set_focal_length_and_outer_hex_radius(
	const double focal_length,
	const double outer_hex_radius
) {
	curvature_radius = 2.0*focal_length;
	this->focal_length = focal_length;
	this->outer_hex_radius = outer_hex_radius;
	restrict_outer_hex_radius_to_curvature_radius();
	hexBounds.set_outer_radius(outer_hex_radius);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void SphereCapWithHexagonalBound::post_initialize_radius_of_enclosing_sphere() {
	
	double sphere_center_to_edge_in_z = 
		sqrt(curvature_radius*curvature_radius - outer_hex_radius*outer_hex_radius);

	radius_of_sphere_enclosing_all_children = hypot(
		outer_hex_radius,
		curvature_radius - sphere_center_to_edge_in_z
	);	
}
//------------------------------------------------------------------------------
void SphereCapWithHexagonalBound::restrict_outer_hex_radius_to_curvature_radius() {
	if(outer_hex_radius > curvature_radius)
		outer_hex_radius = curvature_radius;
}
//------------------------------------------------------------------------------
std::string SphereCapWithHexagonalBound::get_print()const {
	std::stringstream out;
	out << get_frame_print();
	out << get_surface_print();
	out << get_SphereCapWithHexagonalBound_print();
	return out.str();
}
//------------------------------------------------------------------------------
std::string SphereCapWithHexagonalBound::get_SphereCapWithHexagonalBound_print()const {
	std::stringstream out;
	out << "sphere cap with hexagonal bound:\n";
	out << "| focal length: " << focal_length << "m\n";
	out << "| curvature radius: " << curvature_radius << "m\n";
	out << "| outer hex radius: " << outer_hex_radius << "m\n";
	out << "| f/D max: " << focal_length/(2.0*outer_hex_radius) << "\n";
	out << "| frontal projected area: ";
	out << hexBounds.get_projected_area_in_xy_plane() << "m^2\n"; 
	return out.str();
}
//------------------------------------------------------------------------------
const Intersection* SphereCapWithHexagonalBound::calculate_intersection_with(
	const Ray* ray
)const {
	SphericalCapRayIntersectionEquation sphereCapRayEq(curvature_radius, ray);

	if(sphereCapRayEq.has_solutions()) {
		
		return get_causeal_intersection(
			&sphereCapRayEq,
			&hexBounds,
			ray
		);
	}

	return empty_intersection();
}