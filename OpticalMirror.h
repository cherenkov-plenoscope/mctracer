//=================================
// include guard
#ifndef __OPTICALMIRROR_H_INCLUDED__
#define __OPTICALMIRROR_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "SurfaceEntity.h"

using namespace std;

//=================================
class OpticalMirror :public SurfaceEntity{
protected:
	double FocalLength_m;
	double MirrorRadius_m;
public:
	//======================
	bool set_optical_mirror(double new_FocalLength_m,
	double new_MirrorRadius_m
	);
	//======================
	void disp();
	//======================
	string get_optical_mirror_string();
	//======================
	bool test_hexa_fit(Vector3D* vec_intersection)const;
};
#endif // __OPTICALMIRROR_H_INCLUDED__
