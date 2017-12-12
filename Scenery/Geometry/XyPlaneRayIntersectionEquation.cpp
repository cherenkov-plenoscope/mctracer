#include "XyPlaneRayIntersectionEquation.h"
//------------------------------------------------------------------------------
XyPlaneRayIntersectionEquation::XyPlaneRayIntersectionEquation(const Ray* ray) {
	// calculate intersection of  ray: g := sup+v*dir 
	//						and plane: P := a*ex + b*ey
	// g = P 
	// (a,0,0)^T + (0,b,0)^T = (sup_x, sup_y, sup_z)^T + v*(dir_x, dir_y, dir_z)
	// (a,b,0)^T			 = (sup_x + v*dir_x, sup_y+v*dir_y, sup_z+v*dir_z)^T
	//
	// z-component only
	// 0 = sup_z + v*dir_z  <=>  
	// v = -sup_z/dir_z

	if( !ray->direction().is_parallel_to_x_y_plane() ){

		has_valid_intersection = true;
		ray_parameter_v = -ray->get_support().z / ray->direction().z;
	}else{

		has_valid_intersection = false;
	}
}
//------------------------------------------------------------------------------
double XyPlaneRayIntersectionEquation::get_ray_parameter_for_intersection()const {
	return ray_parameter_v;
}
//------------------------------------------------------------------------------
bool XyPlaneRayIntersectionEquation::has_solution()const {
	return has_valid_intersection;
}
//------------------------------------------------------------------------------
bool XyPlaneRayIntersectionEquation::has_causal_solution()const {
	return ray_parameter_v > 0.0 && has_valid_intersection;
}
//------------------------------------------------------------------------------
Vec3 XyPlaneRayIntersectionEquation::get_plane_normal_vector()const {
	return Vec3(0.0, 0.0, 1.0);
}
//------------------------------------------------------------------------------