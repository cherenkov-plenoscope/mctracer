//=================================
// include guard
#ifndef __INTERSECTION_H_INCLUDED__
#define __INTERSECTION_H_INCLUDED__

//=================================
// forward declared dependencies
class CartesianFrame;

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include "Vector3D.h"

//=================================
class Intersection {
private:
// intersection stores all information of a ray intersecting an 
// object. This information is not kept inside the objects to enable
// multi thread, parallel computing without copying all objects for
// each thread.
	const CartesianFrame*	ptr_to_intersecting_object;
	bool 		flag_intersection;
	Vector3D 	intersection_point;
	Vector3D 	surfacenormal_in_intersection_point;
	double 	 	distance_of_ray_in_m;
public:
//==================================================================
const CartesianFrame* get_pointer_to_intersecting_object()const;
//==================================================================
void set_pointer_to_intersecting_object
(const CartesianFrame* new_ptr_to_intersectiong_object);
//==================================================================
Intersection();
//==================================================================
void set_intersection_flag(const bool flag_new_intersection);
//==================================================================
void set_intersection(
	const Vector3D* ptr_to_intersection_vector,
	const Vector3D* ptr_to_intersection_object_surface_normal_vector,
	const double* ptr_to_distance_of_ray );
//==================================================================
bool get_intersection_flag()const;
//==================================================================
void disp()const;
//==================================================================
std::string get_string()const;
//==================================================================
double get_intersection_distance()const;
//======================
void get_reflection_direction_in_object_system(Vector3D* vec)const;
//======================
void get_intersection_vec_in_object_system(Vector3D *inter)const;
};

#endif // __INTERSECTION_H_INCLUDED__ 
