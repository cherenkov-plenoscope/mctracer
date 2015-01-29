#include "Disc.h"
//------------------------------------------------------------------------------
void Disc::set_Disc(const double radius){

	cylinder_bounds.set_radius(radius);
	//set_Disc_radius(radius);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Disc::post_initialize_radius_of_enclosing_sphere(){
	radius_of_sphere_enclosing_all_children = cylinder_bounds.get_radius();
}
//------------------------------------------------------------------------------
void Disc::disp(){

	std::stringstream out;
	out << "Disc:" << name_of_frame << "_________________________________\n";
	out << get_frame_string();
	out << get_surface_print();
	out << get_Disc_string();
	out << "_________________________________\n";
	std::cout << out.str();
}
//------------------------------------------------------------------------------
std::string Disc::get_Disc_string(){
	std::stringstream out;
	out << "||| Disc radius: " << cylinder_bounds.get_radius() << " m\n";
	return out.str();
}
//------------------------------------------------------------------------------
Intersection* Disc::calculate_intersection_with(const Ray* ray)const {

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
				v
			);

			return intersec;
		}
	}
	return empty_intersection();	
}
//------------------------------------------------------------------------------
// old
//------------------------------------------------------------------------------
void Disc::hit(Vector3D *sup, Vector3D *dir, Intersection *intersection)const{
	// hit_flag = false;
	// calculate intersection of  ray: g=b + v*d 
	//						and plane: P=a*ex + b*ey
	// g=P 
	// (a,0,0)^T + (0,b,0)^T = (bx,by,bz)^T + v*(dx,dy,dz)
	// (a,b,0)^T			 = (bx+v*dx, by+v*dy, bz+v*dz)^T
	// z-component
	// 0 = bz+v*dz  <=>  
	// v = -bz/dz   watch out! dz might be 0 !
	if(dir->z() == 0.0){
		// the Disc-surface is parallel to the ray
	}else{
		
		Vector3D surface_normal_ez;
		surface_normal_ez.set_unit_vector_z();
		
		double alpha = -sup->z()/dir->z();

		Vector3D intersec;	
		intersec = *sup + ( *dir)*alpha;
		
		if(	hypot(intersec.x(), intersec.y()) <= cylinder_bounds.get_radius() && alpha > 0.0){
			
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&intersec,
				&surface_normal_ez,
				&alpha
			);
		}
	} 
}
