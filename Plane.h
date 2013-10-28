//=================================
// include guard
#ifndef __PLANE_H_INCLUDED__
#define __PLANE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include "SurfaceEntity.h"
#include "Intersection.h"
//=================================
class Plane  :public SurfaceEntity{
protected:	
	// a plane lies in local x-y plane
	double	range_x_max;
	double	range_x_min;
	double	range_y_max;
	double	range_y_min;
	//for faster tracing use pre allocation
	// the surface normal is always ez
public:
//======================
Plane();
//======================
bool set_plane(	double min_x, double max_x, double min_y, double max_y);
//======================
void disp();
//======================
std::string get_plane_string();
//======================
void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
};
#endif // __PLANE_H_INCLUDED__
