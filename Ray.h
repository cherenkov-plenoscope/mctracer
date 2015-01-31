//=================================
// include guard
#ifndef __RAY_H_INCLUDED__
#define __RAY_H_INCLUDED__

//=================================
// forward declared dependencies
class HomoTrafo3D;
class OctTreeTraversingRay;
//=================================
// included dependencies
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "Vector3D.h"
#include "ColourProperties.h"
#include "CartesianFrame.h"
#include "Intersection.h"
#include "GlobalSettings.h"
#include "OctTreeCube.h"
#include "ListOfInteractions.h"
#include "PseudoRandomNumberGenerator.h"

class Ray{

protected:
	Vector3D support;	
	Vector3D direction;
	ListOfInteractions* history = nullptr;	// interaction history of this ray
	unsigned long long int identifier_number;
	
	void normalize_direction();
public:
	Ray();
	
	Ray(const Vector3D support, const Vector3D direction);

	Ray(const Ray* ray);

	void SetRay(const Vector3D nsup,const Vector3D ndir);	
	
	void SetDirection(const Vector3D ndir);
	
	void SetDirection(const double x,const double y,const double z);
	
	void SetSupport(const Vector3D nsup);
	
	void SetSupport(const double x,const double y,const double z);
	
	void SetID(unsigned long long int nID);

	unsigned long long int ID()const;

	void SetHistory(ListOfInteractions *history);

	ListOfInteractions* GetHistory()const;

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

	void disp()const;
	
	std::string get_string()const;

	bool operator() (Intersection* one, Intersection* two)const;
	
	void operator= (Ray ray);

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

public:
	
	Intersection* get_first_intersection(const CartesianFrame* frame)const;

protected:
	std::vector<const CartesianFrame*> get_intersection_candidate_objects(
		const CartesianFrame* frame
	)const;

	std::vector<Intersection*> get_intersections_in_candidate_objects(
		std::vector<const CartesianFrame*> *candidate_objects
	)const;

	Intersection* get_closest_intersection_and_delete_the_rest(	
		std::vector<Intersection*> *intersections
	)const;


	Intersection* calculate_closest_intersection(
			std::vector<Intersection*> *intersections
	)const;

	bool support_equals_intersection_point(const Intersection* intersec)const;
	// old
public:
	std::vector<Intersection*> get_intersections(
		const CartesianFrame* world,
		const CartesianFrame* object_propagated_from
	)const;

	virtual void propagate(	
		const CartesianFrame* world, 
		ListOfInteractions* history,
		int interaction_count,
		const CartesianFrame* object_propagated_from,
		const GlobalSettings* settings,
		PseudoRandomNumberGenerator* dice
	);

	double get_distance_to_closest_object(
		const CartesianFrame* world,
		const GlobalSettings *settings
	)const;

	void find_intersection_candidates_in_tree_of_frames(
		const CartesianFrame* frame_to_check_for_interaction_of_ray_and_max_sphere, 
		std::vector<const CartesianFrame*> *Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
	)const;

	Intersection* get_closest_intersection(
		const CartesianFrame* world,
		const GlobalSettings *settings
	)const;

	void find_intersections_in_intersection_candidate_frames(
		std::vector<const CartesianFrame*> *objects_which_might_intersect,
		std::vector<Intersection*> *ptr_to_list_of_ptr_to_intersections,
		const CartesianFrame* object_propagated_from
	)const;

	void calculate_reflected_ray(	
		const Intersection * pointer_to_closest_intersection,
		Ray *ray_reflection_on_object
	)const;
};
#endif // __RAY_H_INCLUDED__ 