//=================================
// include guard
#ifndef __SPHERECAPWITHCYLINDERBOUND_H_INCLUDED__
#define __SPHERECAPWITHCYLINDERBOUND_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "SphericalCapRayIntersectionEquation.h"
#include "CylinderPrismZ.h"
#include "SurfaceWithOuterPrismBound.h"

//=================================
class SphereCapWithCylinderBound :public SurfaceWithOuterPrismBound {
protected:	
	CylinderPrismZ CylBounds;

	double focal_length;
	double curvature_radius;
	double cap_radius;
public:

	void set_focal_length_and_cap_radius(
		const double focal_length,
		const double cap_radius
	);

	void print()const;

	Intersection* calculate_intersection_with(const Ray* ray)const;
private:
	void restrict_cap_radius_to_curvature_radius();

	std::string get_SphereCapCylinderBound_print()const;

	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __SPHERECAPWITHCYLINDERBOUND_H_INCLUDED__
