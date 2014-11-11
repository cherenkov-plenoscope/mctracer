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
#include "PseRanNumGen.h"

//=================================
class OctTreeTraversingRay;

class Ray{

protected:
	Vector3D support;                 // support vector
	Vector3D direction;               // direction vector
	ListOfInteractions* history;      // interaction history of this ray
	 // The identifier ID of this ray
	unsigned long long int identifier_number;   

public:
//------------------------------------------------------------------------------
// SET
void SetRay(const Vector3D nsup,const Vector3D ndir);	
void SetDirection(const Vector3D ndir);
void SetDirection(const double x,const double y,const double z);
void SetSupport(const Vector3D nsup);
void SetSupport(const double x,const double y,const double z);
void SetID(unsigned long long int nID);
void SetHistory(ListOfInteractions *nhistory);
//------------------------------------------------------------------------------
// GET
Vector3D Support()const;
Vector3D Direction()const;
Vector3D PositionOnRay(const double scalar)const;
unsigned long long int ID()const;
ListOfInteractions* GetHistory()const;
CsvRow getCsvID()const;
CsvRow getRayCsvRow(GlobalSettings& settings)const;
virtual CsvRow getCsvRow(GlobalSettings& settings)const;
virtual CsvRow getCsvRowHistory(GlobalSettings& settings)const;
//------------------------------------------------------------------------------
// Functionality
Intersection* get_closest_intersection(
	const CartesianFrame* world,
	GlobalSettings *settings
);

void pre_trace(
	const CartesianFrame* frame_to_check_for_interaction_of_ray_and_max_sphere, 
	std::vector<const CartesianFrame*> *Ptr2ListOfFramesWithIntersectionsOfRayAndMaxSpehre
)const;

bool IntersectionWithBoundingSphere(const CartesianFrame* frame)const;

void homo_transformation_of_ray(Ray* ray,const HomoTrafo3D *T)const;

void test_intersection_for_hit_candidates(
	std::vector<const CartesianFrame*> *list_of_objects_which_might_intersect,
	std::vector<Intersection*> *ptr_to_list_of_ptr_to_intersections,
	const CartesianFrame* object_propagated_from,
	int refl_count
)const;

void calculate_reflected_ray(	
	Intersection * pointer_to_closest_intersection,
	Ray *ray_reflection_on_object
);

Intersection* calculate_closest_intersection(	
	std::vector<Intersection*> *pointer_to_list_of_intersections
)const;

ColourProperties trace(
	const CartesianFrame* world,
	int refl_count,
	const CartesianFrame* object_propagated_from,
	GlobalSettings *settings
);

virtual void propagate(	
	const CartesianFrame* world, 
	ListOfInteractions* history,
	int interaction_count,
	const CartesianFrame* object_propagated_from,
	const GlobalSettings* settings,
	PseRanNumGen* dice
);

double get_distance_to_closest_object(
	const CartesianFrame* world,
	const GlobalSettings *settings
)const;
//------------------------------------------------------------------------------
// printing
void disp()const;
void disp_possible_hit_list(const CartesianFrame *frame)const;
std::string get_string()const;
//------------------------------------------------------------------------------
// operators
bool operator() (Intersection* one, Intersection* two)const;
void operator= (Ray ray);
//------------------------------------------------------------------------------
// ostream
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
		support = r->Support();
		direction = r->Direction();
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
		inv_dir.set( 1.0/direction.x() , 1.0/direction.y() , 1.0/direction.z() );

		sign[0] = (inv_dir.x() < 0.0);
		sign[1] = (inv_dir.y() < 0.0);
		sign[2] = (inv_dir.z() < 0.0);
	}
//======================================================================
};
#endif // __RAY_H_INCLUDED__ 
