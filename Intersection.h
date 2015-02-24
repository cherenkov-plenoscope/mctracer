//=================================
// include guard
#ifndef __INTERSECTION_H_INCLUDED__
#define __INTERSECTION_H_INCLUDED__

//=================================
// forward declared dependencies
class Ray;
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

	void get_reflection_direction_in_object_system(Vector3D* vec)const;

	Vector3D get_reflection_direction_in_world_system(
		Vector3D incomming_dir_in_world
	)const;

	bool object_surface_normal_parallel_to_direction_of(const Ray* ray)const;

	bool object_has_outer_surface()const {
		return intersecting_object->has_outer_surface();
	};

	bool object_has_inner_surface()const {
		return intersecting_object->has_inner_surface();
	};

	bool object_has_outer_medium()const {
		return intersecting_object->has_outer_medium();
	};

	bool object_has_inner_medium()const {
		return intersecting_object->has_inner_medium();
	};
};
#endif // __INTERSECTION_H_INCLUDED__ 
