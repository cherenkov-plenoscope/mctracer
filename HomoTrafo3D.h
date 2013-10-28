//=================================
// include guard
#ifndef __HOMOTRAFO3D_H_INCLUDED__
#define __HOMOTRAFO3D_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <iomanip> 
#include "Vector3D.h"
#include "Rotation3D.h"

//=================================
// the actual class
class HomoTrafo3D {
private:
double E[16];
public:
//==================================================================
HomoTrafo3D();
//==================================================================
void set_transformation(const Rotation3D R,const Vector3D pos);
//==================================================================
void transform_orientation(Vector3D* orientation_to_transform) const;
//==================================================================
void transform_position(Vector3D* position_to_transform) const;
//==================================================================
Vector3D get_translation() const;
//==================================================================
void disp() const;
//==================================================================
std::string get_string() const;
//==================================================================
void operator= (HomoTrafo3D G);
//==================================================================
HomoTrafo3D operator* (const HomoTrafo3D G) const;
//==================================================================
HomoTrafo3D inverse() const;
//==================================================================
private:
void 	set_Mat(const int x,const int y,const double dbl);
//==================================================================
double 	get_Mat(const int x,const int y) const;	
};
#endif // __HOMOTRAFO3D_H_INCLUDED__ 
