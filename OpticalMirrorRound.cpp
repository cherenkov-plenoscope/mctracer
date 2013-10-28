#include "OpticalMirrorRound.h"
//======================
bool OpticalMirrorRound::test_zylinder_fit
(Vector3D* vec_intersection)const{
	
	double dbl_dist_ez_axis_to_vec_intersection;
	dbl_dist_ez_axis_to_vec_intersection=
		sqrt(	pow(vec_intersection->get_x(),2.0) +
				pow(vec_intersection->get_y(),2.0));
				
	if(dbl_dist_ez_axis_to_vec_intersection<=MirrorRadius_m)
	{
		// vec_intersection is inside defined volume
		return true;
	}else{
		return false;
	}
}
