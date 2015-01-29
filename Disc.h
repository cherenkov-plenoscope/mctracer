//=================================
// include guard
#ifndef __DISC_H_INCLUDED__
#define __DISC_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "SurfaceEntity.h"
#include "Intersection.h"
#include "XyPlaneRayIntersectionEquation.h"
#include "CylinderPrismZ.h"
//=================================
class Disc :public SurfaceEntity{
protected:
	CylinderPrismZ cylinder_bounds;
public:
	void set_Disc(const double radius);

	void disp();

	std::string get_Disc_string();

	Intersection* calculate_intersection_with(const Ray* ray)const;

	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
private:
	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __DISC_H_INCLUDED__
