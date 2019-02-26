// Copyright 2014 Sebastian A. Mueller
#ifndef SCENERY_GEOMETRY_SURFACEWITHOUTERPRISMBOUND_H_
#define SCENERY_GEOMETRY_SURFACEWITHOUTERPRISMBOUND_H_

#include <vector>
#include "merlict/SurfaceEntity.h"
#include "merlict/Intersection.h"
#include "TwoSolutionSurfaceRayEquation.h"
#include "PrismZ.h"

namespace merlict {

class SurfaceWithOuterPrismBound :public SurfaceEntity{
 protected:
    void add_causeal_intersection(
        const TwoSolutionSurfaceRayEquation* eq,
        const PrismZ* outer_bound,
        const Ray *ray,
        std::vector<Intersection> *intersections)const;
};

}  // namespace relleums

#endif  // SCENERY_GEOMETRY_SURFACEWITHOUTERPRISMBOUND_H_
