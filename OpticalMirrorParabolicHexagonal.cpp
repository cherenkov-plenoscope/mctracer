#include "OpticalMirrorParabolicHexagonal.h"
//======================
OpticalMirrorParabolicHexagonal::OpticalMirrorParabolicHexagonal(){
}
//======================
bool OpticalMirrorParabolicHexagonal::set_parabolic_hexag(double new_FocalLength_m,
double new_MirrorRadius_m
){
	if(set_optical_mirror(
	new_FocalLength_m,
	new_MirrorRadius_m)){
			
		//max norm() radius
		// for parabolic sphere
		radius_of_sphere_enclosing_all_children = 
		MirrorRadius_m*
		sqrt( (1.0 + 1.0/(4.0*FocalLength_m))	);
		return true;
	}else{
		return false;
	}
}
//======================
void OpticalMirrorParabolicHexagonal::disp(){
	std::stringstream out;
	out.str("");
	out<<"parabolic hexagonal mirror:"<<name_of_frame;
	out<<"_________________________________"<<std::endl;
	out<<get_frame_string();
	out<<get_surface_propertie_prompt();
	out<<get_optical_mirror_string();
	out<<"_________________________________"<<std::endl;
	std::cout<<out.str();
}
//======================
std::string OpticalMirrorParabolicHexagonal::get_parabolic_round(){
	std::stringstream out;
	out.str("");
	out<<"||| parabolic mirror:"<<std::endl;
	out<<"||| mirror radius      : "<<MirrorRadius_m<<" [m]"<<std::endl;
	out<<"||| mirror focal length: ";
	out<<FocalLength_m<<" [m]"<<std::endl;
	return out.str();
}
//======================
void OpticalMirrorParabolicHexagonal::hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const{
	// hit_flag = false;
	//
	// <=> (x,y,1/(4*f)*(x^2 + y^2))^T =  (bx,by,bz)^T + t*(dx,dy,dz)^T
	//
	// <=> 1/(4*f)*(bx+t*dx)^2 + 1/(4*f)*(by+t*dy)^2 - bz -t*dz = 0
	//
	// <=> t^2 + t*(2(bx*dx+by*dy)-4fdz)/(dx^2+dy^2) + (bx^2+by^2-4fbz)/(dx^2 + dy^2) =0
	//
	//     	p = 2(bx*dx+by*dy-2fdz)/(dx^2 + dy^2)
	//
	//		q = bx^2+by^2-4f*bz
	//
	/*
	cout<<"base: "<<endl;
	base->disp();
	cout<<"dir: "<<endl;
	dir->disp();
	*/
	double p=(2.0*(	base->x()*dir->x() +
					base->y()*dir->y() )
			-4.0*FocalLength_m*dir->z())/
			(pow(dir->x(),2.0) + pow(dir->y(),2.0));
	
	double q=( pow(base->x(),2.0) + pow(base->y(),2.0)-
			4.0*FocalLength_m*base->z())/
			(pow(dir->x(),2.0) + pow(dir->y(),2.0));
	
	// the inner part of sqrt()
	double dbl_inner_part_of_square_root_p_q =
			pow((p/2.0),2.0) - q;
	
	if(dbl_inner_part_of_square_root_p_q < 0){
		// no hit at all
		//cout<<"no hit at all"<<endl;
	}else{
		// hit in parabola
		//cout<<"hit in big parabola"<<endl;
		double tm=-p/2.0 -sqrt(dbl_inner_part_of_square_root_p_q);
		double tp=-p/2.0 +sqrt(dbl_inner_part_of_square_root_p_q);
		double t=0.0;
		if(tm>0.0){t=tm;}
		if(tp>0.0 && tp<t){t=tp;}
		//cout<<"tm: "<<tm<<" tp: "<<tp<<" t:"<<t<<endl;
		// intersection points
		Vector3D intersec_parabola_tp = *base + *dir *tp;
		Vector3D intersec_parabola_tm = *base + *dir *tm;

		bool hit_valid_tp = false;
		bool hit_valid_tm = false;
		if(test_hexa_fit(&intersec_parabola_tp))
		{
			// intersection inside defined volume
			hit_valid_tp = true;
		}
		if(test_hexa_fit(&intersec_parabola_tm))
		{
			// intersection inside defined volume
			hit_valid_tm = true;
		}
		
		bool hit = false;
		if(hit_valid_tp && hit_valid_tm)
		{
			//both hits are valid
			if(tp>0.0 && tm>0.0)
			{
				//both are pos
				if(tm>=tp){t=tp; hit=true;}else{t=tm; hit=true;}
			}else if(tp>0.0){
				//only tp is pos
				t=tp; hit=true;
			}else if(tm>0.0){
				//only tm is pos
				t=tm; hit=true;
			}else{
				//no one is pos
			}
		}else if(hit_valid_tp)
		{	
			//tp is valid only
			if(tp>0.0){t=tp; hit=true;}
		}else if(hit_valid_tm){
			//tm is valid only
			if(tm>0.0){t=tm; hit=true;}
		}else{
			t=tm;
			//no hit is valid
		}
		
		if(hit){ 
			Vector3D intersec_parabola = *base + *dir *t;
			// surface normal
			Vector3D surface_normal;
			surface_normal.set(
			//x
			-1.0/(2.0*FocalLength_m)*intersec_parabola.x(),
			//y
			-1.0/(2.0*FocalLength_m)*intersec_parabola.y(),
			//z
			1.0
			);
			surface_normal = surface_normal/surface_normal.norm2();
			/*
			hit_flag = true;
			scalar_distance = t;
			vec3D_surfacenormal = surface_normal;
			vec3D_intersection  = intersec_parabola; 
			*/
			// the new intersection feature
			intersection->set_intersection_flag(true);
			intersection->set_pointer_to_intersecting_object(this);
			intersection->set_intersection(
				&intersec_parabola,
				&surface_normal,
				&t
			);	
		}
	}
}
