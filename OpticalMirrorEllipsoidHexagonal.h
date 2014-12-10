
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
private:
    double long_focal_length;
    double short_focal_length;
public:	
    OpticalMirrorEllipsoidHexagonal( );
	
    void set_ellipsoid_hexag( 
        double new_long_focal_length, 
    	double new_short_focal_length,
    	double new_inner_radius_of_hexagonal_shape
    );

    void disp();
   	
	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
private:
    std::string get_ellipsoid_hexag_print()const;

    void assert_named_variable_is_positiv(
        const double variable_to_test, const std::string name_of_variable
    )const;
};
#endif // __MYCLASS_H_INCLUDED__ 
