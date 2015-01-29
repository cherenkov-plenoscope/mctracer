 #include "Plane.h"
//------------------------------------------------------------------------------
void Plane::set_plane_using_x_and_y_width(
	const double x_width, 
	const double y_width
){	
	RectBounds.set_x_y_width(x_width, y_width);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Plane::post_initialize_radius_of_enclosing_sphere() {
	radius_of_sphere_enclosing_all_children = hypot(
		RectBounds.get_half_x_width(), 
		RectBounds.get_half_y_width()
	);
}
//------------------------------------------------------------------------------
void Plane::disp()const {
	std::stringstream out;
	out << "plane:" << name_of_frame << "______________________________\n";
	out << get_frame_string();
	out << get_surface_print();
	out << get_plane_print();
	out << "_________________________________\n";
	std::cout << out.str();
}
//------------------------------------------------------------------------------
std::string Plane::get_plane_print()const {
	std::stringstream out;
	out << "||| x width: " << 2.0*RectBounds.get_half_x_width() << " m \n";
	out << "||| y width: " << 2.0*RectBounds.get_half_y_width() << " m \n";
	return out.str();
}
//------------------------------------------------------------------------------
#include "Ray.h"
#include "Intersection.h"
//------------------------------------------------------------------------------
Intersection* Plane::calculate_intersection_with(const Ray* ray)const {

	XyPlaneRayIntersectionEquation xyPlaneRayEquation(ray);

	if( xyPlaneRayEquation.has_causal_solution() ){
		
		double v = xyPlaneRayEquation.get_ray_parameter_for_intersection();
		Vector3D intersection_vector = ray->PositionOnRay(v);		

		if(	RectBounds.is_inside(&intersection_vector) ) {

			Intersection* intersec;
			intersec = new Intersection(
				this,
				intersection_vector,
				xyPlaneRayEquation.get_plane_normal_vector(),
				v
			);

			return intersec;
		}
	}
	return empty_intersection();	
}
//------------------------------------------------------------------------------
// OLD
//------------------------------------------------------------------------------
void Plane::hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const{
	// hit_flag = false;
	// calculate intersection of  ray: g=b + v*d 
	//						and plane: P=a*ex + b*ey
	// g=P 
	// (a,0,0)^T + (0,b,0)^T = (bx,by,bz)^T + v*(dx,dy,dz)
	// (a,b,0)^T			 = (bx+v*dx, by+v*dy, bz+v*dz)^T
	// z-component
	// 0 = bz+v*dz  <=>  
	// v = -bz/dz   watch out! dz might be 0 !
	if( !dir->is_parallel_to_x_y_plane() ){
		
		double v = -base->z()/dir->z();

		Vector3D intersection_point;		
		intersection_point = *base + ( *dir)*v;

		if(	abs( intersection_point.x() ) <  RectBounds.get_half_x_width() &&
			abs( intersection_point.y() ) <  RectBounds.get_half_y_width() &&
			v > 0.0) 
		{
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&intersection_point,
				&plane_surface_normal,
				&v
			);
		}
	}  
}