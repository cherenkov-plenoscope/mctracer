//=================================
// include guard
#ifndef __SPHERECAPWITHCYLINDERBOUND_H_INCLUDED__
#define __SPHERECAPWITHCYLINDERBOUND_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Scenery/Geometry/SphericalCapRayIntersectionEquation.h"
#include "Scenery/Geometry/CylinderPrismZ.h"
#include "Scenery/Geometry/SurfaceWithOuterPrismBound.h"
#include <vector>
#include <string>

//=================================
class SphereCapWithCylinderBound :public SurfaceWithOuterPrismBound {
protected:	
	CylinderPrismZ CylBounds;

	double curvature_radius;
	double cap_radius;
public:

	void set_curvature_radius_and_outer_radius(
		const double curvature_radius,
		const double cap_radius
	);
	std::string str()const;
	void calculate_intersection_with(
        const Ray* ray, 
        std::vector<Intersection> *intersections
    )const;
    double get_focal_length()const;
private:

	void restrict_cap_radius_to_curvature_radius();
	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __SPHERECAPWITHCYLINDERBOUND_H_INCLUDED__
