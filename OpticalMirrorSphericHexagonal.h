//=================================
// include guard
#ifndef __OPTICALMIRRORSPHERICHEXAGONAL_H_INCLUDED__
#define __OPTICALMIRRORSPHERICHEXAGONAL_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include <vector>
#include "Intersection.h"
#include "OpticalMirrorHexagonal.h"

//=================================
class OpticalMirrorSphericHexagonal :public OpticalMirrorHexagonal{
// f = 1/2 R

	double dbl_curvature_radius;
public:	
	//======================
	OpticalMirrorSphericHexagonal();
	//======================
	bool set_spheric_hexag(double new_FocalLength_m,
	double new_MirrorRadius_m);
	//======================
	void disp();
	//======================
	string get_spherical_hexag_string();
	//======================
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __OPTICALMIRRORSPHERICHEXAGONAL_H_INCLUDED__ 
