 #include "PlaneDualSphericalBound.h"
//------------------------------------------------------------------------------
void PlaneDualSphericalBound::set_x_hight_and_y_width(
	const double x_width, 
	const double y_width
){	
	dual_sphere_bounds.set_x_hight_and_y_width(x_width, y_width);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void PlaneDualSphericalBound::post_initialize_radius_of_enclosing_sphere() {
	bounding_sphere_radius = 
		dual_sphere_bounds.get_enclosing_radius();
}
//------------------------------------------------------------------------------
std::string PlaneDualSphericalBound::get_print()const {
	std::stringstream out;
	out << SurfaceEntity::get_print();
	out << "plane dual circle bound:\n";
	out << "| hight: " << dual_sphere_bounds.get_hight() << "m\n";
	out << "| width: " << dual_sphere_bounds.get_width() << "m\n";
	out << "| circle radius: " << dual_sphere_bounds.get_circle_radius() << "m\n";
	out << "| area: " << dual_sphere_bounds.get_area() << "m^2\n";
	return out.str();
}
//------------------------------------------------------------------------------
#include "Core/Ray.h"
#include "Core/Intersection.h"
//------------------------------------------------------------------------------
void PlaneDualSphericalBound::calculate_intersection_with(
    const Ray* ray, 
    vector<Intersection> *intersections
)const{

	XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);

	if( xyPlaneRayEquation.has_causal_solution() ){
		
		double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
		Vec3 intersection_vector = ray->get_pos_at(v);		

		if(dual_sphere_bounds.is_inside(&intersection_vector) ) {

			if(ray->get_support() != intersection_vector) {

				intersections->push_back(
						Intersection(
						this,
						intersection_vector,
						xyPlaneRayEquation.get_plane_normal_vector(),
						v,
						ray->get_direction()
					)
				);
			}	
		}
	}	
}