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
#include <math.h>
#include <vector>
#include <algorithm>
#include "Core/Vector3D.h"
#include "Core/Frame.h"
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include <gtest/gtest_prod.h>
#include "Core/Printable.h"
using std::string;
using std::stringstream;
using std::vector;

class Ray : public Printable{
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
	void transform(const HomoTrafo3D *T);
	virtual string get_print()const;
	double get_parameter_on_ray_for_closest_distance_to_point(
		const Vector3D &point
	)const;

	

	double get_closest_distance_to_point(const Vector3D &point)const;
	
	Ray get_ray_transformed_in_object_system_of(const Frame* frame)const;
	
	FRIEND_TEST(RayAndFrameInteractionTest, ray_finds_first_interaction); 
	//--------------------------------------------------------------------------
	// Ray and bounding sphere of Frame

	bool support_of_ray_is_inside_bounding_sphere_of(const Frame *frame)const;
	bool has_intersection_with_bounding_sphere_of(const Frame* frame)const;
	//--------------------------------------------------------------------------
	// Ray and Frame
	
	const Intersection* get_first_intersection_in(const Frame* frame)const;

	void find_intersection_candidates_in_tree_of_frames(
		const Frame* frame, 
		vector<const Frame*> *candidate_frames
	)const;
protected:
	Ray();

	void SetRay(const Vector3D nsup,const Vector3D ndir);	

	vector<const Frame*> get_intersection_candidate_objects(
		const Frame* frame
	)const;

	vector<const Intersection*> get_intersections_in_candidate_objects(
		vector<const Frame*> *candidate_objects
	)const;

	const Intersection* get_closest_intersection_and_delete_the_rest(	
		vector<const Intersection*> *intersections
	)const;


	const Intersection* calculate_closest_intersection(
		vector<const Intersection*> *intersections
	)const;

	bool support_equals_intersection_point(const Intersection* intersec)const;
};
#endif // __RAY_H_INCLUDED__ 