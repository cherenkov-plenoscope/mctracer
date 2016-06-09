//=================================
// include guard
#ifndef __PLANE_H_INCLUDED__
#define __PLANE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <string>
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "Scenery/Geometry/XyPlaneRayIntersectionEquation.h"
#include "Scenery/Geometry/RectangularPrismZ.h"
//=================================
class Plane :public SurfaceEntity{
protected:	

	RectangularPrismZ RectBounds;
public:

	Plane() {};
	
	Plane(
		const std::string new_name,
        const Vec3    new_pos,
        const Rot3  new_rot)
		: SurfaceEntity(new_name, new_pos, new_rot) {};

	void set_x_y_width(
		const double x_width,
		const double y_width
	);

	std::string get_print()const;
	void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;
private:
	
	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __PLANE_H_INCLUDED__
