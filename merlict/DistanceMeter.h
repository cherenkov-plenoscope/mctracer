// Copyright 2014 Sebastian A. Mueller
#ifndef MERLICT_DISTANCEMETER_H_
#define MERLICT_DISTANCEMETER_H_

#include "merlict/Ray.h"
#include "merlict/Frame.h"

namespace merlict {

struct DistanceMeter {
    DistanceMeter(const Ray* ray, const std::shared_ptr<Frame> world);
    bool faces_an_object;
    double distance_to_closest_object;
};

}  // namespace merlict

#endif  // MERLICT_DISTANCEMETER_H_
