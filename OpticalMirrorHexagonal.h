//=================================
// include guard
#ifndef __OPTIVALMIRRORHEXAGONAL_H_INCLUDED__
#define __OPTIVALMIRRORHEXAGONAL_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "OpticalMirror.h"

//=================================
class OpticalMirrorHexagonal  :public OpticalMirror{
protected:
	Vector3D vec_ex; 
	Vector3D vec_ey; 
	Vector3D dir_u;
	Vector3D dir_v;
	Vector3D dir_w;
	double dbl_hex_dist_edge2edge;
public:
	//======================
	bool set_optical_hexag(double new_FocalLength_m,
	double new_MirrorRadius_m);
	//======================
	bool test_hexa_fit(Vector3D* vec_intersection)const;
};
#endif // __OPTIVALMIRRORHEXAGONAL_H_INCLUDED__
