// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_BICONVEXLENSHEXBOUND_H_
#define SCENERY_PRIMITIVE_BICONVEXLENSHEXBOUND_H_

#include "merlict/scenery/primitive/SphereCapWithHexagonalBound.h"
#include "merlict/scenery/primitive/PlaneDualSphericalBound.h"

namespace relleums {

class BiConvexLensHexBound :public SurfaceEntity{
 public:
    void set_curvature_radius_and_outer_hex_radius(
        const double curvature_radius,
        const double outer_aperture_radius);

 protected:
    double height_of_a_cap_given_curv_radius_and_outer_radius(
        const double curvature_radius, const double outer_radius)const;
};

}  // namespace relleums

#endif  // SCENERY_PRIMITIVE_BICONVEXLENSHEXBOUND_H_
