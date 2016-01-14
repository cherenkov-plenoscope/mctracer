//=================================
// include guard
#ifndef __TwoSolutionSurfaceRayEquation_H_INCLUDED__
#define __TwoSolutionSurfaceRayEquation_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/Ray.h"
#include "QuadraticEquation.h"
//=================================
class TwoSolutionSurfaceRayEquation {

public:
	virtual bool has_solutions()const = 0;
	virtual double get_minus_solution()const = 0;
	virtual double get_plus_solution()const = 0;
	virtual Vector3D get_surface_normal_given_intersection_vector(
		const Vector3D* intersec
	)const = 0;
};
#endif // __TwoSolutionSurfaceRayEquation_H_INCLUDED__