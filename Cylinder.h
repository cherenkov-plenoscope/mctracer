//=================================
// include guard
#ifndef __CYLINDER_H_INCLUDED__
#define __CYLINDER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "SurfaceEntity.h"
#include "Vector3D.h"
#include "Intersection.h"

using namespace std;

//=================================
class Cylinder :public SurfaceEntity{
protected:
	double CylinderRadius;
	double CylinderLength;
public:
//==================================================================
Cylinder();
//==================================================================
bool set_cylinder(	double new_dbl_cylinder_radius,
					Vector3D vec_start, Vector3D vec_end);
//==================================================================
bool set_cylinder(	double new_dbl_cylinder_radius,
					double new_dbl_cylinder_length);
//==================================================================
void disp();
//==================================================================
std::string get_cylinder_string();
//==================================================================
void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __CYLINDER_H_INCLUDED__
