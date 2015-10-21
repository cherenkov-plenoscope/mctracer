//=================================
// include guard
#ifndef __SPHERE_H_INCLUDED__
#define __SPHERE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include <string>
#include <sstream>
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "QuadraticEquation.h"

//=================================
class Sphere :public SurfaceEntity{
protected:
	
	double radius;
public:

	Sphere();

	void set_sphere_radius(double nradius);

	std::string get_print()const;

	std::string get_sphere_print()const;

	const Intersection* calculate_intersection_with(const Ray* ray)const;
private:

	bool facing_sphere_from_outside_given_p_m(
		const double v_Plus,
		const double v_Minus
	)const;

	bool facing_away_from_outside_given_p_m(
		const double v_Plus,
		const double v_Minus
	)const;

	const Intersection* sphere_intersection_for_ray_parameter(
		const Ray* ray, 
		const double ray_parameter
	)const;

	QuadraticEquation get_ray_parameter_equation_for_intersections_with_sphere(
		const Ray* ray
	)const;
};
#endif // __SPHERE_H_INCLUDED__