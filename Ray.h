//=================================
// include guard
#ifndef __RAY_H_INCLUDED__
#define __RAY_H_INCLUDED__

//=================================
// forward declared dependencies
class HomoTrafo3D;
//class ListOfInteractions;
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
#include "CsvHandler.h"
#include "OctTreeCube.h"
#include "ListOfInteractions.h"

//=================================
class OctTreeTraversingRay;

class Ray{

protected:
	Vector3D base;
	Vector3D dir;
	ListOfInteractions* history;
public:
//======================================================================
CsvRow getRayCsvRow(GlobalSettings& settings)const;
virtual CsvRow getCsvRow(GlobalSettings& settings)const;
//======================================================================
void set_history(ListOfInteractions *nhistory);
ListOfInteractions* get_history()const;
//======================================================================
void set_ray(const Vector3D nbase,const Vector3D ndir);
//======================================================================
void set_base(const double x,const double y,const double z);
//======================================================================
void set_base(const Vector3D nbase);
//======================================================================
void set_dir(const double x,const double y,const double z);
//======================================================================
void set_dir(const Vector3D ndir);
//======================================================================
Vector3D get_support()const;
//======================================================================
Vector3D get_direction()const;
//======================================================================
void disp()const;
//======================================================================
std::string get_string()const;
//======================================================================
Vector3D get_position_on_ray(const double scalar)const;
//======================================================================
void pre_trace(
	const CartesianFrame* frame_to_check_for_interaction_of_ray_and_max_sphere, 
	std::vector<const CartesianFrame*> *Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
)const;
//======================================================================
bool IntersectionWithBoundingSphere(const CartesianFrame* frame)const;
//======================================================================
void disp_possible_hit_list(const CartesianFrame *frame)const;
//======================================================================
void operator= (Ray ray);
//======================================================================
void homo_transformation_of_ray(Ray* ray,const HomoTrafo3D *T)const;
//======================================================================
void test_intersection_for_hit_candidates(
	std::vector<const CartesianFrame*> *list_of_objects_which_might_intersect,
	//std::vector<Intersection*> *ptr_to_list_of_ptr_to_intersections_which_might_take_place,
	std::vector<Intersection*> *ptr_to_list_of_ptr_to_intersections,
	const CartesianFrame* object_reflected_from,
	int refl_count
)const;
//======================================================================
void calculate_reflected_ray(	
	Intersection * pointer_to_closest_intersection,
	Ray *ray_reflection_on_object
);
//======================================================================
Intersection* calculate_closest_intersection(	
	Intersection *pointer_to_closest_intersection,
	std::vector<Intersection*> *pointer_to_list_of_intersections
)const;
//======================================================================
ColourProperties trace(
	const CartesianFrame* world,
	int refl_count,
	const CartesianFrame* object_reflected_from,
	GlobalSettings *settings
);
//======================================================================
virtual void propagate(	
	const CartesianFrame* world, 
	ListOfInteractions* history,
	int interaction_count,
	const CartesianFrame* object_reflected_from,
	const GlobalSettings* settings
);
//======================================================================
double get_distance_to_closest_object(
	const CartesianFrame* world,
	int refl_count,
	CartesianFrame* object_reflected_from,
	const GlobalSettings *settings,
	double dbl_passed_distance_from_source_to_sensor
)const;
//======================================================================
bool operator() (Intersection* one, Intersection* two)const;
//======================================================================
friend std::ostream& operator<<(std::ostream& os, const Ray& ray_to_be_displayed);
};

//======================================================================
// A special Ray to traverse the Oct Tree Structure
//======================================================================
/*here additional signum and inverse direction information are stored 
with in the ray to speed up the Oct Tree traversal. Idea is taken from 

	  An Efficient and Robust Ray–Box Intersection Algorithm
	Amy Williams, Steve Barrus, R. Keith Morley, Peter Shirley
	                   University of Utah
*/

class OctTreeTraversingRay : public Ray{
public:
	Vector3D inv_dir;
	int sign[3];

	OctTreeTraversingRay(const Ray *r){
		base = r->get_support();
		dir  = r->get_direction();
		//update();
	}
//======================================================================
	void IntersectionCandidatesInOctTree(
	const OctTreeCube *Cube,
	std::unordered_set<CartesianFrame*> *IntersectionCandidates)const;
//======================================================================
	bool IntersectionWithOctTreeCube(const OctTreeCube* Cube)const;
//======================================================================
	void update(){
		/* after any usual transformation of the ray this update has to 
		be performed to update the additional information which is not 
		stored or touched with in the usual Ray routines.*/
		inv_dir.set( 1.0/dir.x() , 1.0/dir.y() , 1.0/dir.z() );

		sign[0] = (inv_dir.x() < 0.0);
		sign[1] = (inv_dir.y() < 0.0);
		sign[2] = (inv_dir.z() < 0.0);
	}
//======================================================================
};
#endif // __RAY_H_INCLUDED__ 
