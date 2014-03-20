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
void set_cylinder(	double new_dbl_cylinder_radius,
					Vector3D vec_start, Vector3D vec_end);
//==================================================================
void set_cylinder(	double new_dbl_cylinder_radius,
					double new_dbl_cylinder_length);
//==================================================================
void disp()const;
//==================================================================
std::string get_cylinder_string()const;
//==================================================================
void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
private:
void set_cylinder_radius(double new_cylinder_radius);
};
#endif // __CYLINDER_H_INCLUDED__
