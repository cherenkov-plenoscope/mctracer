#include "Plane.h"
//======================
Plane::Plane(){
	flag_sensor = false;
}
//======================
bool Plane::set_plane(	double min_x, double max_x, double min_y, double max_y){
	//===================
	// set x range
	//===================
	if(max_x > min_x){
		range_x_min=min_x;
		range_x_max=max_x;
	}else{
		std::cout<<"set_plane -> range_x: "<<max_x<<">"<<min_x;
		std::cout<<" is not true!"<<std::endl;
		return false;
	}
	//===================
	// set y range
	//===================
	if(max_y > min_y){
		range_y_min=min_y;
		range_y_max=max_y;
	}else{
		std::cout<<"set_plane -> range_y: "<<max_y<<">"<<min_y;
		std::cout<<" is not true!"<<std::endl;
		return false;
	}
	//===================
	// set radius
	//===================
	std::vector<double> dist_corner_to_base;
	//       y
	//  2    |   1
	//  _____|_____x
	//		 |
	//  3    |   4
	// 1)
	dist_corner_to_base.push_back(	
	sqrt( pow(max_x,2) + pow(max_y,2) ));
	// 2)
	dist_corner_to_base.push_back(	
	sqrt( pow(min_x,2) + pow(max_y,2) ));
	// 3)
	dist_corner_to_base.push_back(	
	sqrt( pow(min_x,2) + pow(min_y,2) ));
	// 4)
	dist_corner_to_base.push_back(	
	sqrt( pow(max_x,2) + pow(min_y,2) ));
	// find corner with largets distance to base
	radius_of_sphere_enclosing_all_children = *max_element(	dist_corner_to_base.begin() ,
						dist_corner_to_base.end() );	
	

	//===================
	return true;
	//===================
}
//======================
void Plane::disp(){
	std::stringstream out;
	out.str("");
	out<<"plane:"<<name_of_frame<<"______________________________"<<std::endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_plane_string();
	out<<"_________________________________"<<std::endl;
	std::cout<<out.str();
}
//======================
std::string Plane::get_plane_string(){
	std::stringstream out;
	out.str("");
	out<<"||| range X: "<<range_x_min<<"<x<"<<range_x_max<<" [m]"<<std::endl;
	out<<"||| range Y: "<<range_y_min<<"<y<"<<range_y_max<<" [m]"<<std::endl;
	return out.str();
}
//======================
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
	if(dir->get_z() == 0.0)
	{
		// the plane is parallel to the ray
		// default hit flag is false
	}else{
		
		Vector3D surface_normal_ez;
		// the surface normal in the objects own frame is always ez.
		surface_normal_ez.set_unit_vector_z();
		Vector3D vec_intersection;
		
		double v ;
		// the ray is not parallel to the plane
		//double v;
		v = -base->get_z()/dir->get_z();
		
		//Vector3D vec_intersection; 
		vec_intersection = *base + ( *dir)*v;
		//vec_intersection.disp();
		
		// test wether intersection is inside 
		// plane definition or not
		if(	vec_intersection.get_x() >  range_x_min &&
			vec_intersection.get_x() <  range_x_max &&
			vec_intersection.get_y() >  range_y_min &&
			vec_intersection.get_y() <  range_y_max	&& 
			v > 0.0) //v must be positiv
		{
			// the intersection
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&vec_intersection,
				&surface_normal_ez,
				&v
			);
		}
	}  
}
