//=================================
// include guard
#ifndef __PlaneDualSphericalBound_H_INCLUDED__
#define __PlaneDualSphericalBound_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <iostream> 
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "Scenery/Geometry/XyPlaneRayIntersectionEquation.h"
#include "Scenery/Geometry/DualSphericalPrismZ.h"
//=================================
class PlaneDualSphericalBound :public SurfaceEntity{
protected:	

	DualSphericalPrismZ dual_sphere_bounds;
public:

	void set_x_hight_and_y_width(
		const double x_width,
		const double y_width
	);

	std::string str()const;
	void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;
private:
	
	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __PlaneDualSphericalBound_H_INCLUDED__
