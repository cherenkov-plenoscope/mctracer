//=================================
// include guard
#ifndef __TRIANGLE_H_INCLUDED__
#define __TRIANGLE_H_INCLUDED__

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

//=================================
class Triangle  :public SurfaceEntity{	

	Vector3D A;
	Vector3D B;	
	Vector3D C;
public:
	void set_corners_in_xy_plane(
		const double Ax, const double Ay,
		const double Bx, const double By,  
		const double Cx, const double Cy
	);

	void set_normal_and_3_vertecies(
		const Vector3D nomal,
		Vector3D a,
		Vector3D b,
		Vector3D c
	);

	std::string get_print()const;

	const Intersection* calculate_intersection_with(const Ray* ray)const;
private:
	void assert_edge_length_is_non_zero(
		const double edge_length, 
		const std::string edge
	)const;

	void post_initialize_radius_of_enclosing_sphere();


	double sign(
		const Vector3D &P1, const Vector3D &P2, const Vector3D &P3
	)const;

	bool is_inside_triangle(const Vector3D &intersec_vec)const;

	std::string get_Triangle_print()const;
}; 
#endif // __TRIANGLE_H_INCLUDED__ 
