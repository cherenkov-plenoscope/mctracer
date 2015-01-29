#include "OpticalMirror.h"
//======================
bool OpticalMirror::set_optical_mirror(double new_FocalLength_m,
double new_MirrorRadius_m
){
	if( new_FocalLength_m > 0 &&
		new_MirrorRadius_m > 0){
			
		FocalLength_m = 
		new_FocalLength_m;

		MirrorRadius_m = new_MirrorRadius_m;
		
		return true;

	}else{
		stringstream out;
		out.str("");
		out<<"Can not initialize optical mirror";
		out<<" with negative radius:"; 
		out<<new_MirrorRadius_m<<" [m]"<<endl;
		out<<"or negative focal length: ";
		out<<new_FocalLength_m<<" [m]"<<endl;
		cout<<out.str();
		return false;
	}
}
//======================
void OpticalMirror::disp(){
	stringstream out;
	out.str("");
	out<<"optical mirror:"<<name_of_frame;
	out<<"_________________________________"<<endl;
	out<<get_frame_string();
	out<<get_surface_print();
	out<<get_optical_mirror_string();
	out<<"_________________________________"<<endl;
	cout<<out.str();
}
//======================
string OpticalMirror::get_optical_mirror_string(){
	stringstream out;
	out.str("");
	out<<"||| mirror radius      : "<<MirrorRadius_m<<" [m]"<<endl;
	out<<"||| mirror focal length: ";
	out<<FocalLength_m<<" [m]"<<endl;
	return out.str();
}
//======================
bool OpticalMirror::test_hexa_fit(Vector3D* vec_intersection)const{
	//init base of hexagonal mirror
	Vector3D vec_ex; vec_ex.set_unit_vector_x();
	Vector3D vec_ey; vec_ey.set_unit_vector_y();
	
	
	
	Vector3D dir_u = vec_ey;
	
	Vector3D dir_v = 
	vec_ex*sin(2.0/3.0*M_PI) + vec_ey*cos(2.0/3.0*M_PI);
	Vector3D dir_w =
	vec_ex*sin(2.0/3.0*M_PI)*(-1.0) + vec_ey*cos(2.0/3.0*M_PI);
	
	//projection onto dir_u
	double dbl_projection_of_intersection_onto_dir_u=
	dir_u* *vec_intersection;
	//projection onto dir_v
	double dbl_projection_of_intersection_onto_dir_v=
	dir_v* *vec_intersection;
	//projection onto dir_w
	double dbl_projection_of_intersection_onto_dir_w=
	dir_w* *vec_intersection;
	
	double dbl_hex_dist_edge2edge = MirrorRadius_m*cos(1.0/6.0*M_PI);
	if(
	dbl_projection_of_intersection_onto_dir_u<dbl_hex_dist_edge2edge
	&&
	dbl_projection_of_intersection_onto_dir_u>-dbl_hex_dist_edge2edge
	&&
	dbl_projection_of_intersection_onto_dir_v<dbl_hex_dist_edge2edge
	&&
	dbl_projection_of_intersection_onto_dir_v>-dbl_hex_dist_edge2edge
	&&
	dbl_projection_of_intersection_onto_dir_w<dbl_hex_dist_edge2edge
	&&
	dbl_projection_of_intersection_onto_dir_w>-dbl_hex_dist_edge2edge
	)
	{
		// vec_intersection is inside defined volume
		return true;
	}else{
		return false;
	}
}

