#include "Disc.h"
//==================================================================
Disc::Disc(){
}
//==================================================================
bool Disc::set_Disc(double dbl_new_Disc_radius_in_m){
	//===================
	// set Disc radius
	//===================
	if(dbl_new_Disc_radius_in_m > 0.0){
		dbl_Disc_radius_in_m = dbl_new_Disc_radius_in_m;
	}else{
		std::cout<<"set_Disc -> Disc_radius: ";
		std::cout<<dbl_new_Disc_radius_in_m<<" <= 0.0";
		std::cout<<" is not valid!"<<std::endl;
		return false;
	}
	// calculate max radius of max_norm
	radius_of_sphere_enclosing_all_children = dbl_Disc_radius_in_m;

	return true;
}
//==================================================================
void Disc::disp(){
	std::stringstream out;
	out.str("");
	out<<"Disc:"<<name_of_frame<<"_________________________________"<<std::endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_Disc_string();
	out<<"_________________________________"<<std::endl;
	std::cout<<out.str();
}
//==================================================================
std::string Disc::get_Disc_string(){
	std::stringstream out;
	out.str("");
	out<<"||| Disc_radius: "<<dbl_Disc_radius_in_m<<" [m]"<<std::endl;
	return out.str();
}
//==================================================================
void Disc::hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const{
	// hit_flag = false;
	// calculate intersection of  ray: g=b + v*d 
	//						and plane: P=a*ex + b*ey
	// g=P 
	// (a,0,0)^T + (0,b,0)^T = (bx,by,bz)^T + v*(dx,dy,dz)
	// (a,b,0)^T			 = (bx+v*dx, by+v*dy, bz+v*dz)^T
	// z-component
	// 0 = bz+v*dz  <=>  
	// v = -bz/dz   watch out! dz might be 0 !
	if(dir->get_z() == 0.0)
	{
		// the Disc-surface is parallel to the ray
		// default hit flag is false
	}else{
		
		Vector3D surface_normal_ez;
		// the surface normal in the objects own frame is ez.
		surface_normal_ez.set_unit_vector_z();
		
		Vector3D vec_intersection;
		double dbl_v ;
		
		// the ray is not parallel to the Disc-surface
		//double v;
		dbl_v = -base->get_z()/dir->get_z();
		
		//Vector3D vec_intersection; 
		vec_intersection = *base + ( *dir)*dbl_v;
		//vec_intersection.disp();
		
		// test wether intersection is inside 
		// Disc definition or not
		if(	sqrt(
				pow(vec_intersection.get_x(),2.0)
				+
				pow(vec_intersection.get_y(),2.0) 
				)
				<= dbl_Disc_radius_in_m &&
			dbl_v > 0.0) //v must be positiv
		{
			// ray is inside range
			// store hit information in SurfaceEntity
			
			// the new intersection feature
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&vec_intersection,
				&surface_normal_ez,
				&dbl_v
			);
		}
	} 
}
