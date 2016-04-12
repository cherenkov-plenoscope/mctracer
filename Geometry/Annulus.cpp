#include "Annulus.h"
//------------------------------------------------------------------------------
void Annulus::set_outer_inner_radius(
		const double outer_radius, 
		const double inner_radius
) {
	outer_bound.set_radius(outer_radius);
	inner_bound.set_radius(inner_radius);

	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Annulus::post_initialize_radius_of_enclosing_sphere(){
	radius_of_sphere_enclosing_all_children = outer_bound.get_radius();
}
//------------------------------------------------------------------------------
std::string Annulus::get_print()const {
	std::stringstream out;
	out << SurfaceEntity::get_print();

	out << "Annulus:\n";
	out << "| Ri: " << inner_bound.get_radius() << "m, ";
	out << "Ro: " << outer_bound.get_radius() << "m\n";
	out << "| area:   " << get_area() << "m^2\n";
	return out.str();
}
//------------------------------------------------------------------------------
double Annulus::get_area()const {
	return M_PI*(
		outer_bound.get_radius()*outer_bound.get_radius() -
		inner_bound.get_radius()*inner_bound.get_radius()
	);
}
//------------------------------------------------------------------------------
const Intersection* Annulus::calculate_intersection_with(const Ray* ray)const {

	XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);

	if( xyPlaneRayEquation.has_causal_solution() ){
		
		double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
		Vec3 intersection_vector = ray->PositionOnRay(v);		

		if(	outer_bound.is_inside(&intersection_vector) && 
			!inner_bound.is_inside(&intersection_vector)
		) {

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