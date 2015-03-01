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

	SphereCapWithCylinderBound* front_cap;
	SphereCapWithCylinderBound* rear_cap;
public:

	void set_curvature_radius_and_diameter(
		const double curvature_radius,
		const double diameter
	);

protected:
	std::string name_of_mother_frame_plus(const std::string appendix)const;

	double height_of_a_cap_given_curv_radius_and_diameter(
		const double curvature_radius, const double diameter
	)const;

	SphereCapWithCylinderBound* create_cap_with_pos_rot_name_focal_lengt_and_diameter(
		const Vector3D pos, 
		const Rotation3D rot, 
		const std::string name,
		const double focal_length, 
		const double diameter
	)const;
};
#endif // __BiConvexLens_H_INCLUDED__ 
