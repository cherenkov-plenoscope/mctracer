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
#include "Core/Vec3.h"
#include "Core/SurfaceEntity.h"

//=================================
class Intersection {
protected:

	Vec3 intersection_point;
	Vec3 surfacenormal_in_intersection_point;
	const SurfaceEntity* object;
	double distance_of_ray;
	bool _from_outside_to_inside;
public:

	static const Intersection void_intersection;
	Intersection();
	Intersection(
		const SurfaceEntity* intersectiong_object,
		const Vec3 intersection_vector,
		const Vec3 surfacenormal_in_intersection_point,
		const double distance_of_ray_support_to_intersection,
		const Vec3 incident_in_obj_sys
	);
	bool does_intersect()const;
	const SurfaceEntity * get_object()const;
	Vec3 get_intersection_vector_in_object_system()const;
	Vec3 get_intersection_vector_in_world_system()const;
	Vec3 get_surface_normal_in_object_system()const;
	Vec3 get_surface_normal_in_world_system()const;
	double get_intersection_distance()const;
	std::string get_print()const;
	void get_reflection_direction_in_object_system(Vec3* vec)const;
	Vec3 get_reflection_direction_in_world_system(
		Vec3 incomming_dir_in_world
	)const;
	double get_facing_reflection_propability(const double wavelength)const;
	double get_refractive_index_going_to(const double wavelength)const;
	double get_refractive_index_coming_from(const double wavelength)const;
	double get_half_way_depth_coming_from(const double wavelength)const;
	double get_half_way_depth_going_to(const double wavelength)const;
	bool boundary_layer_is_transparent()const;
	bool from_outside_to_inside()const;
	const Color get_facing_color()const;
	const HomTra3* object2world()const;
	Vec3 get_normal_in_faceing_surface_system()const;
	bool going_to_default_refractive_index()const;
	bool operator<(const Intersection& other) const;
	static bool compare(const Intersection &one, const Intersection &two);
protected:

	bool ray_is_running_from_outside_to_inside(
		const Vec3 incident_in_obj_sys
	)const;
};
#endif // __INTERSECTION_H_INCLUDED__ 
