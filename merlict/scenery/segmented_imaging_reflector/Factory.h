// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_SCENERY_SEGMENTED_IMAGING_REFLECTOR_FACTORY_H_
#define MERLICT_SCENERY_SEGMENTED_IMAGING_REFLECTOR_FACTORY_H_

#include <vector>
#include "merlict/scenery/segmented_imaging_reflector/Geometry.h"
#include "merlict/SurfaceEntity.h"

namespace merlict {
namespace segmented_imaging_reflector {

class Factory {
    const Config cfg;
    Geometry geometry;
    std::vector<std::shared_ptr<Frame>> facets;
 public:
    explicit Factory(const Config ncfg);
    void add_to_SurfaceEntity(SurfaceEntity* reflector);
};

}  // namespace segmented_imaging_reflector
}  // namespace merlict

#endif  // MERLICT_SCENERY_SEGMENTED_IMAGING_REFLECTOR_FACTORY_H_
