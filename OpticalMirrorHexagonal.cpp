#include "OpticalMirrorHexagonal.h"
//======================
bool OpticalMirrorHexagonal::set_optical_hexag(
double new_FocalLength_m,
double new_MirrorRadius_m
){
	// SLOW
	if(set_optical_mirror(	new_FocalLength_m,
							new_MirrorRadius_m))
	{
		//init base of hexagonal mirror
		vec_ex.set_unit_vector_x();
		vec_ey.set_unit_vector_y();
		
		dir_u = vec_ey;
		
		dir_v = 
		vec_ex*sin(2.0/3.0*M_PI) + vec_ey*cos(2.0/3.0*M_PI);
		dir_w =
		vec_ex*sin(2.0/3.0*M_PI)*(-1.0) + vec_ey*cos(2.0/3.0*M_PI);
		
		dbl_hex_dist_edge2edge =MirrorRadius_m*cos(1.0/6.0*M_PI);
		return true;
	}else{
		return false;
	}
}
//======================
bool  OpticalMirrorHexagonal::test_hexa_fit(
Vector3D* vec_intersection
)const{
	//projection onto dir_u
	double dbl_projection_of_intersection_onto_dir_u=
	dir_u* *vec_intersection;
	//projection onto dir_v
	double dbl_projection_of_intersection_onto_dir_v=
	dir_v* *vec_intersection;
	//projection onto dir_w
	double dbl_projection_of_intersection_onto_dir_w=
	dir_w* *vec_intersection;
	//cout<<"dbl_hex_dist_edge2edge: "<<dbl_hex_dist_edge2edge<<endl;
	
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

