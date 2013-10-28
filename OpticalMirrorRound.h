//=================================
// include guard
#ifndef __OPTICALMIRRORROUND_H_INCLUDED__
#define __OPTICALMIRRORROUND_H_INCLUDED__

//=================================
// forward declared dependencies


//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "OpticalMirror.h"

//=================================
class OpticalMirrorRound  :public OpticalMirror{
public:
	//======================
	bool test_zylinder_fit(Vector3D* vec_intersection)const;
};
#endif // __OPTICALMIRRORROUND_H_INCLUDED__
