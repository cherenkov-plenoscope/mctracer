//=================================
// include guard
#ifndef __SurfaceWithOuterPrismBound_H_INCLUDED__
#define __SurfaceWithOuterPrismBound_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Core/SurfaceEntity.h"
#include "Core/Intersection.h"
#include "TwoSolutionSurfaceRayEquation.h"
#include "PrismZ.h"

//=================================
class SurfaceWithOuterPrismBound :public SurfaceEntity{
protected:

	const Intersection* get_causeal_intersection(
		const TwoSolutionSurfaceRayEquation* eq,
		const PrismZ* outer_bound,
		const Ray *ray
	)const;
};
#endif // __SurfaceWithOuterPrismBound_H_INCLUDED__
