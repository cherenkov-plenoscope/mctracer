//=================================
// include guard
#ifndef __HEXPLANE_H_INCLUDED__
#define __HEXPLANE_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/SurfaceEntity.h"
#include "XyPlaneRayIntersectionEquation.h"
#include "HexagonalPrismZ.h"

//=================================
class HexPlane :public SurfaceEntity{
protected:	

	HexagonalPrismZ hex_bounds;
	const Vector3D plane_surface_normal = Vector3D::unit_z;
public:

	using SurfaceEntity::SurfaceEntity;
	void set_outer_hex_radius(const double outer_hex_radius);
	std::string get_print()const;
	const Intersection* calculate_intersection_with(const Ray* ray)const;
private:

	void post_initialize_radius_of_enclosing_sphere();
};
#endif // __HEXPLANE_H_INCLUDED__
