//=================================
// include guard
#ifndef __MYCLASS_H_INCLUDED__
#define __MYCLASS_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "Intersection.h"
#include "OpticalMirrorRound.h"

//=================================
class OpticalMirrorParabolicRound :public OpticalMirrorRound{
public:
	//======================
	OpticalMirrorParabolicRound();
	//======================
	bool set_parabolic_round(double new_FocalLength_m,
	double new_MirrorRadius_m);
	//======================
	void disp();
	//======================
	std::string get_parabolic_round();
	//======================
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __MYCLASS_H_INCLUDED__ 
