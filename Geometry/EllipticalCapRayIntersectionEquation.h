//=================================
// include guard
#ifndef __EllipticalCapRayIntersectionEquation_H_INCLUDED__
#define __EllipticalCapRayIntersectionEquation_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "TwoSolutionSurfaceRayEquation.h"
//=================================
class EllipticalCapRayIntersectionEquation : public TwoSolutionSurfaceRayEquation {

	const double A, B, C, iAA, iBB, iCC;

	double plus_solution;
	double minus_solution;
	bool _has_solutions = false;
public:
	EllipticalCapRayIntersectionEquation(
		const double _A, const double _B,  const double _C,
		const double _iAA, const double _iBB,  const double _iCC,  
		const Ray* ray
	);
 	bool has_solutions()const;
	double get_minus_solution()const;
	double get_plus_solution()const;
	Vec3 get_surface_normal_given_intersection_vector(
		const Vec3* intersec
	)const;
};
#endif // __EllipticalCapRayIntersectionEquation_H_INCLUDED__