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
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "XyPlaneRayIntersectionEquation.h"
#include "RectangularPrismZ.h"
//=================================
class Plane :public SurfaceEntity{
protected:	
	RectangularPrismZ RectBounds;

	const Vector3D plane_surface_normal = Vector3D(0.0,0.0,1.0);
public:

	void set_plane_using_x_and_y_width(
		const double x_width,
		const double y_width
	);

	std::string get_print()const;

	const Intersection* calculate_intersection_with(const Ray* ray)const;
private:
	std::string get_plane_print()const;

	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __PLANE_H_INCLUDED__
