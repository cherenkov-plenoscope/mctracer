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
#include <limits>
#include "Vector3D.h"
#include "SurfaceEntity.h"

//=================================
class Intersection {
protected:
	const SurfaceEntity*	intersecting_object;
	Vector3D 	intersection_point;
	Vector3D 	surfacenormal_in_intersection_point;
	double 	 	distance_of_ray_in_m;
	bool _from_outside_to_inside;

	static const SurfaceEntity* void_object;
public:
	Intersection();

	Intersection(
		const SurfaceEntity* intersectiong_object,
		const Vector3D intersection_vector,
		const Vector3D surfacenormal_in_intersection_point,
		const double distance_of_ray_support_to_intersection,
		const Vector3D incident_in_obj_sys
	);

	bool does_intersect()const;

	const SurfaceEntity * get_intersecting_object()const;

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

	double get_facing_reflection_propability()const;
	double get_facing_reflection_propability(const double wavelength)const;

	double get_outer_half_way_depth()const;
	double get_outer_half_way_depth(const double wavelength)const;
	double get_inner_half_way_depth()const;
	double get_inner_half_way_depth(const double wavelength)const;

	double get_refractive_index_going_to()const;
	double get_refractive_index_going_to(const double wavelength)const;
	double get_refractive_index_coming_from()const;
	double get_refractive_index_coming_from(const double wavelength)const;

	bool boundary_layer_is_transparent()const;
	bool from_outside_to_inside()const;

	const ColourProperties get_facing_color()const;

	const HomoTrafo3D* world2object()const;
	const HomoTrafo3D* object2world()const;

	Vector3D get_normal_in_faceing_surface_system()const;

protected:
	bool ray_is_running_from_outside_to_inside(
		const Vector3D incident_in_obj_sys
	)const;
};
#endif // __INTERSECTION_H_INCLUDED__ 
