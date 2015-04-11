//=================================
// include guard
#ifndef __CYLINDER_H_INCLUDED__
#define __CYLINDER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "Core/SurfaceEntity.h"
#include "Core/Vector3D.h"
#include "Core/Intersection.h"
#include "ZaxisCylinderRayIntersectionEquation.h"

//=================================
class Cylinder :public SurfaceEntity{
protected:
	double Radius;
	double Length;
public:

	void set_cylinder(
		const double radius, 
		const Vector3D start_pos, 
		const Vector3D end_pos
	);

	void set_cylinder(const double radius, const double length);

	std::string get_print()const;

	std::string get_cylinder_print()const;

	const Intersection* calculate_intersection_with(const Ray* ray)const;
private:
	void set_cylinder_length(const double Length);

	void set_cylinder_length(const Vector3D start_pos, const Vector3D end_pos);

	void set_cylinder_radius(const double new_cylinder_radius);

	void post_initialize_radius_of_enclosing_sphere();

	void assert_start_and_end_point_are_distinct(
		const Vector3D start_pos, const Vector3D end_pos
	)const;

	void set_position_and_orientation(
		const Vector3D start_pos,
		const Vector3D end_pos
	);

	bool is_in_cylinders_z_bounds(const Vector3D* vec)const;

	Vector3D get_surface_normal_for_intersection_vec(const Vector3D* vec)const;

	Rotation3D calculate_new_rotation_in_mother(const Vector3D rotsym_axis)const;
};
#endif // __CYLINDER_H_INCLUDED__
