//=================================
// include guard
#ifndef __BiConvexLens_H_INCLUDED__
#define __BiConvexLens_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "SphereCapWithCylinderBound.h"

//=================================
class BiConvexLens :public SurfaceEntity{
public:

	void set_curvature_radius_and_aperture_radius(
		const double curvature_radius,
		const double aperture_radius
	);
protected:

	double height_of_a_cap_given_curv_radius_and_aperture_radius(
		const double curvature_radius, const double aperture_radius
	)const;
};
#endif // __BiConvexLens_H_INCLUDED__ 