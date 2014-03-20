#include "OpticalMirrorSphericRound.h"
//==================================================================
OpticalMirrorSphericRound::OpticalMirrorSphericRound(){
}
//======================
bool OpticalMirrorSphericRound::set_spheric_round(double new_FocalLength_m,
double new_MirrorRadius_m){

	if(set_optical_mirror(	new_FocalLength_m,
							new_MirrorRadius_m)){
			
		// in a spherical mirror the radius of the mirror measured 
		// when viewing directly on its front cant be bigger then
		// the actual sphere radius which is two times the focal
		// length
		
		if(new_MirrorRadius_m <= 2.0*FocalLength_m){
			MirrorRadius_m = new_MirrorRadius_m;
		}else{
			MirrorRadius_m = 2.0*FocalLength_m;
		}
		
		//max norm() radius not best solution but ...
		dbl_curvature_radius = 2.0*FocalLength_m;
		
		radius_of_sphere_enclosing_all_children = 
		sqrt(
			pow(MirrorRadius_m,2.0) 
			+ 
			pow(
				(dbl_curvature_radius 
				-
				sqrt(pow(dbl_curvature_radius,2.0) )	)
			,2.0)
		);
		return true;
	}else{
		return false;
	}
}
//======================
void OpticalMirrorSphericRound::disp(){
	stringstream out;
	out.str("");
	out<<"spherical round mirror:"<<name_of_frame;
	out<<"_________________________________"<<endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_spherical_round_string();
	out<<"_________________________________"<<endl;
	cout<<out.str();
}
//======================
std::string OpticalMirrorSphericRound::get_spherical_round_string(){
	stringstream out;
	out.str("");
	out<<"||| spherical mirror:"<<endl;
	out<<"||| mirror radius      : ";
	out<<MirrorRadius_m<<" [m]"<<endl;
	out<<"||| mirror focal length: ";
	out<<FocalLength_m<<" [m]"<<endl;
	out<<"||| mirror curvature radius: ";
	out<<dbl_curvature_radius<<" [m]"<<endl;
	return out.str();
}
//======================
void OpticalMirrorSphericRound::hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const{
	// hit_flag = false;
	// sphere intersection with ray
	// z(x,y) = R-sqrt(R^2 -x^2 -y^2) |- R, *(-1)
	// R - z  = sqrt(R^2 -x^2 -y^2)   |pow2
	// (R-z)^2= R^2 - x^2 - y^2       |+x^2 +y^2
	// R^2 -2Rz +z^2 +y^2 +x^2 = R^2  |-R^2
	// x^2 + y^2 + z^2 -2Rz = 0       | x=bx+v*dx,y=by+v*dy,z=bz+v*dz
	// v^2(dTd) + v 2(bTd -Rdz) +bTb -2Rbz = 0
	
	//calculate dTd
	double dbl_dirTdir;
	dbl_dirTdir = 	(*dir)*(*dir);
	
	//calculate bTb
	double dbl_baseTbase;
	dbl_baseTbase = (*base)*(*base);
	
	//calculate bTd
	double dbl_baseTdir;
	dbl_baseTdir = (*base)*(*dir);
	
	//calculate p of pq equation
	double p; p=2.0*
	(dbl_baseTdir-dbl_curvature_radius*dir->get_z())/dbl_dirTdir;
	
	//calculate q of pq equation
	double q; q=
	(dbl_baseTbase-2.0*dbl_curvature_radius*base->get_z())/dbl_dirTdir;
	
	//calculate the inner part of sqrt()
	double dbl_inner_part_of_square_root_p_q;
	dbl_inner_part_of_square_root_p_q =
			pow((p/2.0),2.0) - q;

	//intersection parameters

	
	if(dbl_inner_part_of_square_root_p_q < 0){
		// no hit at all

	}else{
		// hit in sphere
		
		Vector3D vec_intersec_sphere_tp;
		Vector3D vec_intersec_sphere_tm;
		
		double tm;
		double tp;
		double t;
		
		bool hit_valid_tp;
		bool hit_valid_tm;
		bool hit_inside_zylinder_flag;
		
		tm=-p/2.0 -sqrt(dbl_inner_part_of_square_root_p_q);
		tp=-p/2.0 +sqrt(dbl_inner_part_of_square_root_p_q);
		t=0.0;

		// intersection points of ray and parabola
		vec_intersec_sphere_tp = *base + *dir *tp;
		vec_intersec_sphere_tm = *base + *dir *tm;
		
		//test wether intersections are inside defined zylinder 
		// or not
		hit_valid_tp = false;
		hit_valid_tm = false;
		
		if(test_zylinder_fit(&vec_intersec_sphere_tp))
		{
			// intersection inside defined volume
			hit_valid_tp = true;
		}
		if(test_zylinder_fit(&vec_intersec_sphere_tm))
		{
			// intersection inside defined volume
			hit_valid_tm = true;
		}
		
		
		
		hit_inside_zylinder_flag = false;
		if(hit_valid_tp && hit_valid_tm)
		{
			//both hits are valid
			if(tp>0.0 && tm>0.0)
			{
				//both are pos
				if(tm>=tp){t=tp; hit_inside_zylinder_flag=true;
				}else{t=tm; hit_inside_zylinder_flag=true;}
			}else if(tp>0.0){
				//only tp is pos
				t=tp; hit_inside_zylinder_flag=true;
			}else if(tm>0.0){
				//only tm is pos
				t=tm; hit_inside_zylinder_flag=true;
			}else{
				//no one is pos
			}
		}else if(hit_valid_tp)
		{	
			//tp is valid only
			if(tp>0.0){t=tp; hit_inside_zylinder_flag=true;}
		}else if(hit_valid_tm){
			//tm is valid only
			if(tm>0.0){t=tm; hit_inside_zylinder_flag=true;}
		}else{
			t=tm;
			//no hit is valid
		}
		
		//intersection vector
		Vector3D vec_intersec_sphere;
		//surface normal
		double dbl_surf_norm_factor;
		Vector3D surface_normal;
		
		if(hit_inside_zylinder_flag){ 
			vec_intersec_sphere = *base + *dir *t;
			// surface normal on a sphere
			// z(x,y)= R -( R^2 -x^2 -y^2 )^1/2
			// dz/dx = 1/2( R^2 -x^2 -y^2 )^-1/2 * (-2x)
			// dz/dy = 1/2( R^2 -x^2 -y^2 )^-1/2 * (-2y)
			
			dbl_surf_norm_factor = 
			-pow(
				pow(dbl_curvature_radius	,2.0)-
				pow(vec_intersec_sphere.get_x()	,2.0)-
				pow(vec_intersec_sphere.get_y()	,2.0)
			,-0.5);
			

			surface_normal.set(
			//x
			vec_intersec_sphere.get_x()*dbl_surf_norm_factor,
			//y
			vec_intersec_sphere.get_y()*dbl_surf_norm_factor,
			//z
			1.0
			);
			surface_normal = surface_normal/surface_normal.norm2();
			/*
			hit_flag = true;
			scalar_distance = t;
			vec3D_surfacenormal = surface_normal;
			vec3D_intersection  = vec_intersec_sphere; 	
			*/
			// the new intersection feature
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&vec_intersec_sphere,
				&surface_normal,
				&t
			);
			
		}
	}
}
