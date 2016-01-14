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
#include "Geometry/Cylinder.h"
#include "Geometry/Sphere.h"

//=================================
class TrajectoryFactory {
	const RayForPropagation* ray;
	double radius_of_trajectory_in_m;
	uint intersection_index;
	uint num_of_interactions;
public:

	static const Color* trajectory_col;
	static const Color* absorption_in_void_col;
	static const Color* interaction_col;	
	TrajectoryFactory(const RayForPropagation* ray);
	Frame* get_trajectory()const;
	void set_trajectory_radius(const double radius);
private:

	Frame* get_empty_trajectory_frame()const;
	std::string get_trajectory_frame_name()const;
	std::string get_trajectory_of_part_index(const uint part_index)const;
	std::string get_intersection_point_name_of_part(const uint part_index)const;
	Cylinder* get_trajectory_line_of_part(const uint part_index)const;
	Sphere* get_intersection_indicator_of_part(const uint part_index)const;
	bool is_not_the_last_intersection(const uint part_index)const;
};
#endif // __TrajectoryFactory_H_INCLUDED__