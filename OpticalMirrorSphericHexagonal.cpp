#include "OpticalMirrorSphericHexagonal.h"
//======================
OpticalMirrorSphericHexagonal::OpticalMirrorSphericHexagonal(){
}
//======================
bool OpticalMirrorSphericHexagonal::set_spheric_hexag(double new_FocalLength_m,
double new_MirrorRadius_m
){
	if(set_optical_hexag(	new_FocalLength_m,
							new_MirrorRadius_m)){
		// cout<<MirrorRadius_m<<endl;	
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
void OpticalMirrorSphericHexagonal::disp(){
	stringstream out;
	out.str("");
	out<<"spherical hexagonal mirror:"<<name_of_frame;
	out<<"_________________________________"<<endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_spherical_hexag_string();
	out<<"_________________________________"<<endl;
	cout<<out.str();
}
//======================
string OpticalMirrorSphericHexagonal::get_spherical_hexag_string(){
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
void OpticalMirrorSphericHexagonal::hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const{
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
	
	if(dbl_inner_part_of_square_root_p_q < 0){
		// no hit at all

	}else{
		// hit in parabola

		//intersection parameters
		double tm;
		double tp;
		double t;
		Vector3D vec_intersec_sphere_tp;
		Vector3D vec_intersec_sphere_tm;

		//intersection flags
		bool hit_valid_tp;
		bool hit_valid_tm;
		bool hit_inside_cylinder_flag;
		//intersection vector
		Vector3D vec_intersec_sphere;
		//surface normal
		double dbl_surf_norm_factor;
		Vector3D surface_normal;

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
		
		hit_valid_tp = test_hexa_fit(&vec_intersec_sphere_tp);
		hit_valid_tm = test_hexa_fit(&vec_intersec_sphere_tm);
		
		//we do only want to see the lower part of the 
		//sphere and not the part on the top of it.
		//this is a crude approxymation for thin mirrors.
		
		if(vec_intersec_sphere_tm.get_z()>radius_of_sphere_enclosing_all_children)
			hit_valid_tm = false;
		
		if(vec_intersec_sphere_tp.get_z()>radius_of_sphere_enclosing_all_children)
			hit_valid_tp = false;
		
		// we only want to see objects in front of the camera
		// tp and tm must therefore be positiv
		
		if(tm <= 0.0)
			hit_valid_tm = false;
		
		if(tp <= 0.0)
			hit_valid_tp = false;
			
		hit_inside_cylinder_flag = false;
		if(hit_valid_tp && hit_valid_tm)
		{
			// the tp solution is covered by the tm solution
			t=tm; 
			hit_inside_cylinder_flag=true;
		}else if(hit_valid_tp)
		{	
			//tp is valid only
			t=tp; 
			hit_inside_cylinder_flag=true;
		}else if(hit_valid_tm){
			//tm is valid only
			t=tm; 
			hit_inside_cylinder_flag=true;
		}else{
			//no hit is valid
			return;
		}
		
		if(hit_inside_cylinder_flag){ 
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

