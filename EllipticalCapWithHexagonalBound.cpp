#include "EllipticalCapWithHexagonalBound.h"
//------------------------------------------------------------------------------
void EllipticalCapWithHexagonalBound::set_focalLength_radiiRatio_hexAngel_hexRadius(
	const double _focal_length,
	const double _ratio_x_to_y_curvature_radius,
	const double _hex_bound_rotation,
	const double _outer_hex_radius
) {
	focal_length =_focal_length;
	ratio_x_to_y_curvature_radius = _ratio_x_to_y_curvature_radius;
	hex_bound_rotation = _hex_bound_rotation;
	outer_hex_radius = _outer_hex_radius;

	const double mean_curvature_radius = focal_length*0.5;
	X_curvature_radius = mean_curvature_radius*ratio_x_to_y_curvature_radius;
	Y_curvature_radius = mean_curvature_radius/ratio_x_to_y_curvature_radius;
	Z_curvature_radius = 0.5*(X_curvature_radius + Y_curvature_radius);

	iXX = 1.0/(X_curvature_radius*X_curvature_radius);
	iYY = 1.0/(Y_curvature_radius*Y_curvature_radius);
	iZZ = 1.0/(Z_curvature_radius*Z_curvature_radius);

	restrict_outer_hex_radius_to_curvature_radius();
	hexBounds.set_outer_radius(outer_hex_radius);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void EllipticalCapWithHexagonalBound::restrict_outer_hex_radius_to_curvature_radius() {
	if(outer_hex_radius > Z_curvature_radius)
		outer_hex_radius = Z_curvature_radius;
}
//------------------------------------------------------------------------------
void EllipticalCapWithHexagonalBound::post_initialize_radius_of_enclosing_sphere() {

	double sphere_center_to_edge_in_z = 
		sqrt(Z_curvature_radius*Z_curvature_radius - outer_hex_radius*outer_hex_radius);

	radius_of_sphere_enclosing_all_children = hypot(
		outer_hex_radius,
		Z_curvature_radius - sphere_center_to_edge_in_z
	);
}
//------------------------------------------------------------------------------
std::string EllipticalCapWithHexagonalBound::get_print()const {
	std::stringstream out;
	out << SurfaceEntity::get_print();

	out << "elliptical cap with hexagonal bound:\n";
	out << "| focal length: " << focal_length << "m\n";
	out << "| curvature radius X, Y, ratio: " << X_curvature_radius << "m, ";
	out << Y_curvature_radius << "m, " << ratio_x_to_y_curvature_radius << "\n";
	out << "| outer hex radius: " << outer_hex_radius << "m\n";
	out << "| f/D max: " << focal_length/(2.0*outer_hex_radius) << "\n";
	out << "| frontal projected area: ";
	out << hexBounds.get_area() << "m^2\n"; 
	return out.str();		
}
//------------------------------------------------------------------------------
const Intersection* EllipticalCapWithHexagonalBound::calculate_intersection_with(
	const Ray* ray
)const {
	EllipticalCapRayIntersectionEquation ellipCapRayEq(
		X_curvature_radius, Y_curvature_radius, Z_curvature_radius,
		iXX, iYY, iZZ,
		ray
	);

	if(ellipCapRayEq.has_solutions()) {
		
		return get_causeal_intersection(
			&ellipCapRayEq,
			&hexBounds,
			ray
		);
	}

	return empty_intersection();
}