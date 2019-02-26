// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_SCENERY_SEGMENTED_IMAGING_REFLECTOR_FACTORY_H_
#define CORE_SCENERY_SEGMENTED_IMAGING_REFLECTOR_FACTORY_H_

#include <vector>
#include "Core/scenery/segmented_imaging_reflector/Geometry.h"
#include "Core/SurfaceEntity.h"

namespace relleums {
namespace segmented_imaging_reflector {

class Factory {
    const Config cfg;
    Geometry geometry;
    std::vector<Frame*> facets;
 public:
    explicit Factory(const Config ncfg);
    void add_to_SurfaceEntity(SurfaceEntity* reflector);
};

}  // namespace segmented_imaging_reflector
}  // namespace relleums

#endif  // CORE_SCENERY_SEGMENTED_IMAGING_REFLECTOR_FACTORY_H_
