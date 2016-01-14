#include "Disc.h"
//------------------------------------------------------------------------------
void Disc::set_radius(const double radius){

	cylinder_bounds.set_radius(radius);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Disc::post_initialize_radius_of_enclosing_sphere(){
	radius_of_sphere_enclosing_all_children = cylinder_bounds.get_radius();
}
//------------------------------------------------------------------------------
std::string Disc::get_print()const {
	std::stringstream out;
	out << SurfaceEntity::get_print();

	out << "disc:\n";
	out << "| radius: " << cylinder_bounds.get_radius() << "m\n";
	out << "| area:   " << get_area() << "m^2\n";
	return out.str();
}
//------------------------------------------------------------------------------
double Disc::get_area()const {
	return cylinder_bounds.get_radius()*cylinder_bounds.get_radius()*M_PI;
}
//------------------------------------------------------------------------------
const Intersection* Disc::calculate_intersection_with(const Ray* ray)const {

	XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);

	if( xyPlaneRayEquation.has_causal_solution() ){
		
		double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
		Vector3D intersection_vector = ray->PositionOnRay(v);		

		if(	cylinder_bounds.is_inside(&intersection_vector) ) {

			Intersection* intersec;
			intersec = new Intersection(
				this,
				intersection_vector,
				xyPlaneRayEquation.get_plane_normal_vector(),
				v,
				ray->Direction()
			);

			return intersec;
		}
	}
	return empty_intersection();	
}