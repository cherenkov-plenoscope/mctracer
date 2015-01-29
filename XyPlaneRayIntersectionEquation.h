//=================================
// include guard
#ifndef __XYPLANEANDRAYINTERSECTIONEQUATION_H_INCLUDED__
#define __XYPLANEANDRAYINTERSECTIONEQUATION_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Ray.h"
//=================================
class XyPlaneRayIntersectionEquation {
	double ray_parameter_v;
	bool has_valid_intersection;
public:
	XyPlaneRayIntersectionEquation(const Ray* ray);
	bool has_solution()const;
	bool has_causal_solution()const;
	double get_ray_parameter_for_intersection()const;
	Vector3D get_plane_normal_vector()const;
};
#endif // __XYPLANEANDRAYINTERSECTIONEQUATION_H_INCLUDED__