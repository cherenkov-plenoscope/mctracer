//=================================
// include guard
#ifndef __OPTICALMIRRORSPHERICROUND_H_INCLUDED__
#define __OPTICALMIRRORSPHERICROUND_H_INCLUDED__

//=================================
// forward declared dependencies
class Foo;
class Bar;

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "OpticalMirrorRound.h"
#include "Intersection.h"

//=================================
class OpticalMirrorSphericRound :public OpticalMirrorRound{

	double dbl_curvature_radius;// f = 1/2 R
public:
	//==================================================================
	OpticalMirrorSphericRound();
	//======================
	bool set_spheric_round(double new_FocalLength_m,
	double new_MirrorRadius_m);
	//======================
	void disp();
	//======================
	std::string get_spherical_round_string();
	//======================
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __OPTICALMIRRORSPHERICROUND_H_INCLUDED__
