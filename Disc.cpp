#include "Disc.h"
//------------------------------------------------------------------------------
void Disc::set_Disc(const double Radius){

	set_Disc_radius(Radius);
	post_initialize_radius_of_enclosing_sphere();
}
//------------------------------------------------------------------------------
void Disc::set_Disc_radius(const double Radius){

	if(Radius > 0.0){
		this->Radius = Radius;
	}else{
		std::stringstream info;
		info << "Cylinder::" << __func__ << "()\n";
		info << "The radius of a disc must be larger than 0.0m !\n";
		info << "Expected: >0.0, but actual: " << Radius << "\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
void Disc::post_initialize_radius_of_enclosing_sphere(){
	radius_of_sphere_enclosing_all_children = Radius;
}
//------------------------------------------------------------------------------
void Disc::disp(){

	std::stringstream out;
	out << "Disc:" << name_of_frame << "_________________________________\n";
	out << get_frame_string();
	out << get_surface_propertie_prompt();
	out << get_Disc_string();
	out << "_________________________________\n";
	std::cout << out.str();
}
//------------------------------------------------------------------------------
std::string Disc::get_Disc_string(){
	std::stringstream out;
	out << "||| Disc radius: " << Radius << " m\n";
	return out.str();
}
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
		
		if(	hypot(intersec.x(), intersec.y()) <= Radius && alpha > 0.0){
			
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
