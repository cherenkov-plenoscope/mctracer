//=================================
// include guard
#ifndef __HEXPLANE_H_INCLUDED__
#define __HEXPLANE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/SurfaceEntity.h"
#include "Scenery/Geometry/XyPlaneRayIntersectionEquation.h"
#include "Scenery/Geometry/HexagonalPrismZ.h"

//=================================
class HexPlane :public SurfaceEntity{
protected:	

	HexagonalPrismZ hex_bounds;
	static const Vec3 plane_surface_normal;
public:

	using SurfaceEntity::SurfaceEntity;
	void set_outer_hex_radius(const double outer_hex_radius);
	std::string str()const;
	void calculate_intersection_with(
        const Ray* ray, 
        vector<Intersection> *intersections
    )const;
private:

	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __HEXPLANE_H_INCLUDED__
