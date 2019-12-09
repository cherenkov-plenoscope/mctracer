// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_PRIMITIVE_BICONVEXLENS_H_
#define SCENERY_PRIMITIVE_BICONVEXLENS_H_

#include "SphereCapWithCylinderBound.h"

namespace merlict {

class BiConvexLens : public SurfaceEntity {
 public:
    void set_curvature_radius_and_aperture_radius(
        const double curvature_radius,
        const double aperture_radius);

 protected:
    double height_of_a_cap_given_curv_radius_and_aperture_radius(
        const double curvature_radius, const double aperture_radius)const;
};

}  // namespace merlict

#endif  // SCENERY_PRIMITIVE_BICONVEXLENS_H_
