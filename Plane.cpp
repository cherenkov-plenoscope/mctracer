 #include "Plane.h"
//------------------------------------------------------------------------------
void Plane::set_plane_using_x_and_y_width(
	const double x_width, 
	const double y_width
){	
	set_planes_x_width(x_width);
	set_planes_y_width(y_width);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Plane::set_planes_x_width(const double x_width){
	assert_width_is_positive(x_width,"x");
	half_x_width = 0.5*x_width;
}
//------------------------------------------------------------------------------
void Plane::set_planes_y_width(const double y_width){
	assert_width_is_positive(y_width,"y");
	half_y_width = 0.5*y_width;
}
//------------------------------------------------------------------------------
void Plane::assert_width_is_positive(const double width, const std::string dim
)const{

	if( width <= 0.0 ){

		std::stringstream info;
		info << "Plane::" << __func__ << "()\n";
		info << "The " << dim << "_width must not be smaller or equal zero! ";
		info << "Expected: " << dim << "_width > 0, but actual: " << dim;
		info << "_width =" << width << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void Plane::post_initialize_radius_of_enclosing_sphere(){
	radius_of_sphere_enclosing_all_children = hypot(half_x_width, half_y_width);
}
//------------------------------------------------------------------------------
void Plane::disp(){
	std::stringstream out;
	out << "plane:" << name_of_frame << "______________________________\n";
	out << get_frame_string();
	out << get_surface_propertie_prompt();
	out << get_plane_print();
	out << "_________________________________\n";
	std::cout << out.str();
}
//------------------------------------------------------------------------------
std::string Plane::get_plane_print(){
	std::stringstream out;
	out << "||| x width: " << 2.0*half_x_width << " m \n";
	out << "||| y width: " << 2.0*half_y_width << " m \n";
	return out.str();
}
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

		if(	abs( intersection_point.x() ) <  half_x_width &&
			abs( intersection_point.y() ) <  half_y_width &&
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