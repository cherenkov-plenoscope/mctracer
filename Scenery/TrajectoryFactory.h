//=================================
// include guard
#ifndef __TrajectoryFactory_H_INCLUDED__
#define __TrajectoryFactory_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/RayForPropagation.h"
#include "Core/SurfaceEntity.h"
#include "Scenery/Primitive/Cylinder.h"
#include "Scenery/Primitive/Sphere.h"

//=================================
// Spectate the propagation history of a photon.
// A set of geometry is created from the photons propagation history.
// Cylinders are placed along the straight parts of flight and balls in the 
// interaction points.
//
// Input:
//    Single Photon* (Ray for Propagation)
//
// Output:
//    A Frame* representing the photons path of flight 
// 
class TrajectoryFactory {

	const RayForPropagation* ray;
	double radius_of_trajectory;
	uint intersection_index;
	uint num_of_interactions;
	Frame* trajectory;
public:

	static const Color trajectory_col;
	static const Color absorption_in_void_col;
	static const Color interaction_col;

	TrajectoryFactory(const RayForPropagation* ray);
	void set_trajectory_radius(const double radius);
	void append_trajectory_to(Frame* root_frame);
	void erase_trajectory_from(Frame* root_frame);
private:

	std::string get_trajectory_of_part_index(const uint part_index)const;
	std::string get_intersection_point_name_of_part(const uint part_index)const;
	bool is_not_the_last_intersection(const uint part_index)const;
};
#endif // __TrajectoryFactory_H_INCLUDED__