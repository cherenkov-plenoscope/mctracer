//=================================
// include guard
#ifndef __SphereCapWithRectangularBound_H_INCLUDED__
#define __SphereCapWithRectangularBound_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "SphericalCapRayIntersectionEquation.h"
#include "RectangularPrismZ.h"
#include "SurfaceWithOuterPrismBound.h"

//=================================
class SphereCapWithRectangularBound :public SurfaceWithOuterPrismBound {
protected:	
	
	RectangularPrismZ rect_bounds;
	double curvature_radius;
public:

	void set_curvature_radius_and_x_y_width(
		const double _curvature_radius,
		const double x_width,
		const double y_width
	);
	std::string get_print()const;
	void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;
    double get_focal_length()const;
private:

	void restrict_cap_radius_to_curvature_radius();
	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __SphereCapWithRectangularBound_H_INCLUDED__
