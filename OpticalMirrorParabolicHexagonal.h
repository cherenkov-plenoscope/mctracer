//=================================
// include guard
#ifndef __OPTICALMIRRORPARABOLICHEXAGONAL_H_INCLUDED__
#define __OPTICALMIRRORPARABOLICHEXAGONAL_H_INCLUDED__

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
class OpticalMirrorParabolicHexagonal :public OpticalMirrorRound{
public:
	//======================
	OpticalMirrorParabolicHexagonal();
	//======================
	bool set_parabolic_hexag(double new_FocalLength_m,
	double new_MirrorRadius_m);
	//======================
	void disp();
	//======================
	std::string get_parabolic_round();
	//======================
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __OPTICALMIRRORPARABOLICHEXAGONAL_H_INCLUDED__ 
