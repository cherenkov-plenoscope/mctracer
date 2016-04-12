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
#include "Core/Vec3.h"
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

	Vec3 support;	
	Vec3 direction;
public:

	Ray(const Vec3 support, const Vec3 direction);
	void SetDirection(const Vec3 ndir);
	void SetSupport(const Vec3 nsup);
	Vec3 Support()const;
	Vec3 Direction()const;
	Vec3 PositionOnRay(const double scalar)const;
	void transform(const HomTra3 *T);
	virtual string get_print()const;
	double get_parameter_on_ray_for_closest_distance_to_point(
		const Vec3 &point
	)const;

	

	double get_closest_distance_to_point(const Vec3 &point)const;
	
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

	void SetRay(const Vec3 nsup,const Vec3 ndir);	

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