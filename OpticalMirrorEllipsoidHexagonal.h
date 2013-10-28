
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
#include "OpticalMirrorHexagonal.h"

//=================================
// the actual class
class OpticalMirrorEllipsoidHexagonal :public OpticalMirrorHexagonal{
public:
    double long_focal_length;
    double short_focal_length;
    //double inner_radius_of_hexagonal_shape;
	//==================================================================
    OpticalMirrorEllipsoidHexagonal( );
	//==================================================================
    bool set_ellipsoid_hexag( 
    double new_long_focal_length, 
	double new_short_focal_length,
	double new_inner_radius_of_hexagonal_shape);
	//==================================================================
    void disp();
   	//==================================================================
    string get_spherical_hexag_string();
	//==================================================================
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __MYCLASS_H_INCLUDED__ 
