//=================================
// include guard
#ifndef __Annulus_H_INCLUDED__
#define __Annulus_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "Scenery/Geometry/XyPlaneRayIntersectionEquation.h"
#include "Scenery/Geometry/CylinderPrismZ.h"
//=================================
class Annulus :public SurfaceEntity{
protected:

	CylinderPrismZ outer_bound;
	CylinderPrismZ inner_bound;
public:

	void set_outer_inner_radius(
		const double outer_radius, 
		const double inner_radius
	);
	std::string str()const;
	void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;
private:

	double get_area()const;
	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __Annulus_H_INCLUDED__
