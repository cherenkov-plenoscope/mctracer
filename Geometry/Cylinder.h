//=================================
// include guard
#ifndef __CYLINDER_H_INCLUDED__
#define __CYLINDER_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/SurfaceEntity.h"
#include "Core/Vec3.h"
#include "Core/Intersection.h"
#include "ZaxisCylinderRayIntersectionEquation.h"

//=================================
class Cylinder :public SurfaceEntity{
protected:

	double Radius;
	double Length;
public:

	Cylinder();
	Cylinder(const string name, const Vec3 pos, const Rot3 rot);
	void set_cylinder(
		const double radius, 
		const Vec3 start_pos, 
		const Vec3 end_pos
	);
	void set_radius_and_length(const double radius, const double length);
	string get_print()const;
	void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;
private:

	void set_cylinder_length(const double Length);
	void set_cylinder_length(const Vec3 start_pos, const Vec3 end_pos);
	void set_cylinder_radius(const double new_cylinder_radius);
	void post_initialize_radius_of_enclosing_sphere();
	void assert_start_and_end_point_are_distinct(
		const Vec3 start_pos, const Vec3 end_pos
	)const;
	void set_position_and_orientation(
		const Vec3 start_pos,
		const Vec3 end_pos
	);
	bool is_in_cylinders_z_bounds(const Vec3* vec)const;
	Vec3 get_surface_normal_for_intersection_vec(const Vec3* vec)const;
	Rot3 calculate_new_rotation_in_mother(const Vec3 rotsym_axis)const;
public:

    class BadRadius :public TracerException{
        using TracerException::TracerException;
    };

    class BadStartEndPoints :public TracerException{
        using TracerException::TracerException;
    };

    class BadLength :public TracerException{
        using TracerException::TracerException;
    };
};
#endif // __CYLINDER_H_INCLUDED__
