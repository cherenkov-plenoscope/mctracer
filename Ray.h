//=================================
// include guard
#ifndef __RAY_H_INCLUDED__
#define __RAY_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include "Vector3D.h"
#include "CartesianFrame.h"
#include "SurfaceEntity.h"
#include "Intersection.h"
#include "GlobalSettings.h"

class Ray{

protected:
	Vector3D support;	
	Vector3D direction;
public:
	Ray(const Vector3D support, const Vector3D direction);

	void SetDirection(const Vector3D ndir);

	void SetSupport(const Vector3D nsup);

	Vector3D Support()const;
	
	Vector3D Direction()const;
	
	Vector3D PositionOnRay(const double scalar)const;
	
	double get_distance_to_point_from_position_of_ray_at(
		const Vector3D &point, const double ray_parameter_for_position_on_ray
	)const;

	void transform(const HomoTrafo3D *T);

	void homo_transformation_of_ray(Ray* ray,const HomoTrafo3D *T)const;

	double get_parameter_on_ray_for_closest_distance_to_point(
		const Vector3D &point
	)const;

	double get_closest_distance_to_point(const Vector3D &point)const;

	virtual std::string get_print()const;

	std::string get_ray_print()const;

	bool operator() (const Intersection* one, const Intersection* two)const;
	
	Ray get_ray_transformed_in_object_system_of(const CartesianFrame* frame)const;
	
	friend std::ostream& operator<<(std::ostream& os, const Ray& ray_to_be_displayed);
	//--------------------------------------------------------------------------
	// Ray and bounding sphere of Frame

	bool support_of_ray_is_inside_bounding_sphere_of(
		const CartesianFrame *frame
	)const;

	bool has_intersection_with_bounding_sphere_of(
		const CartesianFrame* frame
	)const;
	//--------------------------------------------------------------------------
	// Ray and Frame
	
	const Intersection* get_first_intersection_in(const CartesianFrame* frame)const;

	void find_intersection_candidates_in_tree_of_frames(
		const CartesianFrame* frame, 
		std::vector<const CartesianFrame*> *candidate_frames
	)const;

	void calculate_reflected_ray(	
		const Intersection * intersec,
		Ray *ray_reflection_on_object
	)const; 
protected:
	Ray();

	void SetRay(const Vector3D nsup,const Vector3D ndir);	

	std::vector<const CartesianFrame*> get_intersection_candidate_objects(
		const CartesianFrame* frame
	)const;

	std::vector<const Intersection*> get_intersections_in_candidate_objects(
		std::vector<const CartesianFrame*> *candidate_objects
	)const;

	const Intersection* get_closest_intersection_and_delete_the_rest(	
		std::vector<const Intersection*> *intersections
	)const;


	const Intersection* calculate_closest_intersection(
			std::vector<const Intersection*> *intersections
	)const;

	bool support_equals_intersection_point(const Intersection* intersec)const;

	void normalize_direction();
};
#endif // __RAY_H_INCLUDED__ 