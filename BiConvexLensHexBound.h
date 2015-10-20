//=================================
// include guard
#ifndef __BiConvexLensHexBound_H_INCLUDED__
#define __BiConvexLensHexBound_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "SphereCapWithHexagonalBound.h"

//=================================
class BiConvexLensHexBound :public SurfaceEntity{

	SphereCapWithHexagonalBound* front_cap;
	SphereCapWithHexagonalBound* rear_cap;
public:

	void set_curvature_radius_and_outer_hex_radius(
		const double curvature_radius,
		const double outer_radius
	);

protected:
	
	double height_of_a_cap_given_curv_radius_and_outer_radius(
		const double curvature_radius, const double outer_radius
	)const;

	SphereCapWithHexagonalBound* create_cap_with_pos_rot_name_focal_lengt_and_outer_radius(
		const Vector3D pos, 
		const Rotation3D rot, 
		const std::string name,
		const double focal_length, 
		const double outer_radius
	)const;
};
#endif // __BiConvexLensHexBound_H_INCLUDED__ 