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
class Plane :public SurfaceEntity{
protected:	
	double half_x_width;
	double half_y_width;

	const Vector3D plane_surface_normal = Vector3D(0.0,0.0,1.0);
	std::string get_plane_print();
public:
	void set_plane_using_x_and_y_width(
		const double x_width,
		const double y_width
	);

	void disp();

	void hit(Vector3D *base,Vector3D *dir, Intersection *intersection)const;
private:
	void set_planes_x_width(const double x_width);

	void set_planes_y_width(const double y_width);

	void assert_width_is_positive(
		const double width, 
		const std::string dim
	)const;

	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __PLANE_H_INCLUDED__
