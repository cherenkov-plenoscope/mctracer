//=================================
// include guard
#ifndef __INTERSECTION_H_INCLUDED__
#define __INTERSECTION_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include "Vector3D.h"
#include "CartesianFrame.h"

//=================================
class Intersection {
private:
	const CartesianFrame*	intersecting_object;
	Vector3D 	intersection_point;
	Vector3D 	surfacenormal_in_intersection_point;
	double 	 	distance_of_ray_in_m;

public:
	Intersection();

	Intersection(
		const CartesianFrame* intersectiong_object,
		const Vector3D intersection_vector,
		const Vector3D surfacenormal_in_intersection_point,
		const double distance_of_ray_support_to_intersection
	);

	bool does_intersect()const;

	const CartesianFrame * get_intersecting_object()const;

	Vector3D get_intersection_vector_in_object_system()const;

	Vector3D get_intersection_vector_in_world_system()const;

	Vector3D get_surface_normal_in_object_system()const;
	
	Vector3D get_surface_normal_in_world_system()const;

	double get_intersection_distance()const;

	std::string get_print()const;
//------------------------------------------------------------------------------
// OLD
//------------------------------------------------------------------------------
	void get_reflection_direction_in_object_system(Vector3D* vec)const;
}; 
#endif // __INTERSECTION_H_INCLUDED__ 
