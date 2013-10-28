#include "Sphere.h"
//======================
Sphere::Sphere(){
	flag_sensor = false;
}
//======================
bool Sphere::set_sphere(double new_radius){
	if(new_radius > 0){
		radius = new_radius;
		radius_of_sphere_enclosing_all_children = new_radius;
		return true;
	}else{
		std::stringstream out;
		out.str("");
		out<<"Can not initialize sphere with negative radius: ";
		out<<new_radius<<" [m]"<<std::endl;
		std::cout<<out.str();
		return false;
	}
}
//======================
void Sphere::disp(){
	std::stringstream out;
	out.str("");
	out<<"sphere:"<<name_of_frame;
	out<<"_________________________________"<<std::endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_sphere_string();
	out<<"_________________________________"<<std::endl;
	std::cout<<out.str();
}
//======================
std::string Sphere::get_sphere_string(){
	std::stringstream out;
	out.str("");
	out<<"||| radius of sphere: "<<radius<<std::endl;
	return out.str();
}
//======================
void Sphere::hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const{	
	// hit_flag = false;
	// FAST
	// 		r = sqrt( x^2 + y^2 + z^2 )
	// 		g:  b + v*d
	// 		put g into sphere eq
	// 		r = sqrt((bx+v*dx)^2 + (by+v*dy)^2 + (bz+v*dz)^2 )
	// 		r = sqrt( v^2 (dd) + v 2(bd) + (bb) )
	// 	  r^2 = v^2 (dd) + v 2(bd) + (bb)
	// 	   	0 = v^2 + v 2(bd/dd) + (bb/dd -r^2)
	//		solve quadrativ eqaution in v

	double p;
	p = 2*( (*base * *dir) / (*dir * *dir) );
	double q;
	q = (*base * *base) / (*dir * *dir) -pow(radius,2.0);
	double inside_square_root_in_p_q_eq ;
	inside_square_root_in_p_q_eq = pow((p/2),2.0)-q;
	
	if( inside_square_root_in_p_q_eq  >= 0){
		// at least one hit
		double v_P;
		double v_M;
		
		Vector3D vec_intersection;
		Vector3D vec_surface_normal;
		
		v_P = -p/2 + sqrt(inside_square_root_in_p_q_eq);
		v_M = -p/2 - sqrt(inside_square_root_in_p_q_eq);
		if(v_P >= 0.0 && v_M >= 0.0){
			// facing the sphere from outside
			//cout<<"facing the sphere from outside."<<endl;
			

			
			vec_intersection = *base + *dir *v_M;
			vec_surface_normal =
			vec_intersection/vec_intersection.norm2();
			
			// store hit information in SurfaceEntity
			/*
			hit_flag = true;
			scalar_distance = v_M;
			vec3D_surfacenormal = vec_surface_normal;
			vec3D_intersection  = vec_intersection;
			*/
			// the new intersection feature
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&vec_intersection,
				&vec_surface_normal,
				&v_M
			);
			
			
		}else{
			if(v_P < 0.0 && v_M < 0.0){
				// looking away from the sphere from outside
				//cout<<"looking away from the sphere from outside."<<endl;
			}else{
				// camera inside of sphere
				
				vec_intersection = *base + *dir *v_P;
				vec_surface_normal =
				vec_intersection/vec_intersection.norm2();
				
				// store hit information in SurfaceEntity
				/*
				hit_flag = true;
				scalar_distance = v_P;
				vec3D_surfacenormal = vec_surface_normal;
				vec3D_intersection  = vec_intersection;
				*/
				// the new intersection feature
				intersection->set_intersection_flag(true);
				intersection->set_pointer_to_intersecting_object(this);
				intersection->set_intersection(
					&vec_intersection,
					&vec_surface_normal,
					&v_P
				);
				
				//cout<<"camera inside of sphere. "<<endl;
			}
		}
	} 
}
